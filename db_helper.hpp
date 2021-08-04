#pragma once





#include <iostream>
#include <conio.h>
#include <map>
#include <functional>
#include "exceptions.hpp"
#include "serialized_integral.hpp"
#include "sqlite3.h"
#include "container.hpp"
#include "string_utilities.hpp"


struct db_helper_context {
    container mapper;
    std::vector<container> results;
};

class db_helper {
public:
    db_helper(){}
    ~db_helper(){}

    void open(const std::string szPath = ":memory:")
    {
        std::scoped_lock lock(mux);
        if (m_db != nullptr) {
            // If db is already active, do nothing
            return;
        }
        int rc = sqlite3_open(szPath.c_str(), &m_db);
        if (rc) {
            throw system_exception("sql error: " + std::string(sqlite3_errmsg(m_db)));            
        }
    }

    std::vector<container> get(std::string sql, container rowmapper) 
    {
        std::scoped_lock lock(mux);
        check_db();

        db_helper_context context;
        context.mapper = rowmapper;
        char* zErrMsg = 0;
        int rc = sqlite3_exec(m_db, sql.c_str(), callback, static_cast<void*>(&context), &zErrMsg);
        
        if (rc != SQLITE_OK) {
            std::string  szErr = zErrMsg;
            sqlite3_free(zErrMsg);
            throw system_exception("sql error: " + szErr);
        }
        else {
            return context.results;
        }
    };

    static int callback(void* data, int argc, char** argv, char** azColName) 
    {

        db_helper_context* context = static_cast<db_helper_context*>(data);
        container c = context->mapper;
        std::string szColumn;
        for (unsigned int i = 0; i < (unsigned int)argc; i++) {
            szColumn = lower(azColName[i]);
            if (context->mapper.exists(szColumn)) {
                serialized_integral val = (argv[i] ? serialized_integral(context->mapper.get(szColumn).getType(), std::string(argv[i])) : serialized_integral());
                c.define(szColumn, val, true);
            }
        }
        context->results.push_back(c);
        return 0;
    }


    std::vector<container> run_prepared(
        const std::string& sql,                 // sql string to execute
        std::vector<serialized_integral> args,  // arguments to provide as parameters
        container mapper,                    // prototype of resulting row objects
        bool include_unexpected,                // whether or not to include in resulting object columns that do not have a corresponding value in the mapper
        int numBinds = 1)                       // number of times to bind and execute the query
    {
        std::scoped_lock lock(mux);
        check_db();
        std::vector<container> results;

        sqlite3_stmt* stmt = nullptr;
        try {
            int rc = 0;
            rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, 0);
            if (rc != SQLITE_OK) {
                throw runtime_exception("sql error: 1" + std::string(sqlite3_errmsg(m_db)));
            }

            for (int bindIndex = 0; bindIndex < numBinds; bindIndex++) {
                unsigned int bind_parameter_count = sqlite3_bind_parameter_count(stmt);
                if (bind_parameter_count != args.size()) {
                    throw runtime_exception("sql error: expected " + std::to_string(bind_parameter_count) + " binds but got " + std::to_string(args.size()));
                }

                for (unsigned int i = 0; i < bind_parameter_count; i++) {

                    //  Bind-parameter indexing is 1-based.
                    if (args.at(i).getType() == si_types().INT()) {
                        rc = sqlite3_bind_int(stmt, i + 1, int_or_error(args.at(i)));
                    }
                    else if (args.at(i).getType() == si_types().DOUBLE()) {
                        rc = sqlite3_bind_double(stmt, i + 1, double_or_error(args.at(i)));
                    }
                    else if (args.at(i).getType() == si_types().NULLTYPE()) {
                        rc = sqlite3_bind_null(stmt, i + 1);
                    }
                    else if (args.at(i).getType() == si_types().STRING()) {
                        rc = sqlite3_bind_text(stmt, i + 1, args.at(i).getValue().c_str(), strlen(args.at(i).getValue().c_str()), nullptr);
                    }
                    else {
                        throw runtime_exception("sql error: unsupported parameter type; object was: " + args.at(i).toString());
                    }
                    if (rc != SQLITE_OK) {
                        throw runtime_exception("sql error: 2" + std::string(sqlite3_errmsg(m_db)));
                    }
                }

                //  Read results
                rc = sqlite3_step(stmt);
                while (rc == SQLITE_ROW) { // While query has result rows.
                    container row = mapper;
                    for (int colIndex = 0; colIndex < sqlite3_column_count(stmt); colIndex++) {
                        std::string szColumnName = lower(sqlite3_column_name(stmt, colIndex));
                        int columnType = sqlite3_column_type(stmt, colIndex); // SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB OR SQLITE_NULL
                        if (include_unexpected) {
                            if (columnType == SQLITE_INTEGER) {
                                row.define(szColumnName, serialized_integral(sqlite3_column_int(stmt, colIndex)), true);
                            }
                            else if (columnType == SQLITE_FLOAT) {
                                row.define(szColumnName, serialized_integral(sqlite3_column_double(stmt, colIndex)), true);
                            }
                            else if (columnType == SQLITE_TEXT) {
                                std::string value = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, colIndex)));
                                row.define(szColumnName, serialized_integral(value, false), true);
                            }
                            else if (columnType == SQLITE_NULL) {
                                row.define(szColumnName, serialized_integral(), true);
                            }
                            else {
                                throw runtime_exception("sql error: unsupported column return type of column " + szColumnName);
                            }
                        }
                        else {
                            if (row.exists(szColumnName)) {
                                if (columnType == SQLITE_INTEGER) {
                                    row.assign(szColumnName, serialized_integral(sqlite3_column_int(stmt, colIndex)), true);
                                }
                                else if (columnType == SQLITE_FLOAT) {
                                    row.assign(szColumnName, serialized_integral(sqlite3_column_double(stmt, colIndex)), true);
                                }
                                else if (columnType == SQLITE_TEXT) {
                                    row.assign(szColumnName, serialized_integral(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, colIndex))), false), true);
                                }
                                else if (columnType == SQLITE_NULL) {
                                    row.assign(szColumnName, serialized_integral(), true);
                                }
                                else {
                                    throw runtime_exception("sql error: unsupported column return type of column " + szColumnName);
                                }
                            }
                            else {
                                throw runtime_exception("sql error: unable to map column '" + szColumnName + "' to value");
                            }
                        }
                    }
                    results.push_back(row);
                    rc = sqlite3_step(stmt);
                }
                if (rc != SQLITE_DONE) {
                    throw runtime_exception("sql error: 3" + std::string(sqlite3_errmsg(m_db)));
                }
                //  Reset the statement after each bind.
           
                rc = sqlite3_reset(stmt);
                if (rc != SQLITE_OK) {
                    throw runtime_exception("sql error: 4" + std::string(sqlite3_errmsg(m_db)));
                }
            }
           
            rc = sqlite3_finalize(stmt);  //  Finalize the prepared statement.
            if (rc != SQLITE_OK) {
                throw runtime_exception("sql error: 6" + std::string(sqlite3_errmsg(m_db)));
            }
            return results;
        }
        catch (runtime_exception re) {
            sqlite3_finalize(stmt);
            throw re;
        }
        catch (system_exception se) {
            sqlite3_finalize(stmt);
            throw se;
        }
    }

    long long query_count() const { return m_queryCount; }

private:

    void check_db()
    {
        if (m_db == nullptr) {
            throw system_exception("db was null");
        }
        m_queryCount++;
    }

    sqlite3* m_db{ nullptr };
    std::mutex mux;
    std::map<std::string, std::string> m_mapper;
    long long m_queryCount{ 0 };
};
