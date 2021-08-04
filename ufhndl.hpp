#pragma once


#ifndef __INCLUDE__PRELIM__UNIVERSAL_FILE_HANDLE_H
#define __INCLUDE__PRELIM__UNIVERSAL_FILE_HANDLE_H

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "exceptions.hpp"

class FileHandle
{
public:
	FileHandle();
	~FileHandle();
	void copyFile(const std::string& source, const std::string& dest);
	bool removeAll(const std::string& source);
	std::vector<std::string> readFile(const std::string& szFilepath);
	std::string readFileAsString(const std::string& filepath);
	void writeToFile(const std::string& filepath, std::vector<std::string> data);
	std::string readLine(const std::string& filepath, unsigned int lineNumber);
	unsigned int countLines(const std::string& filepath);

	std::vector<std::string> split(std::string str, char splitOn);
	std::vector<std::string> splitOnFirst(std::string str, char splitOn);

	static std::string current_path()
	{
		return std::filesystem::current_path().string();
	}

	static std::string parent_path(const std::string& szFilePath)
	{
		std::filesystem::path fp(szFilePath);
		return fp.parent_path().string();
	}

	static void rename(const std::string& szSource, const std::string& szRename)
	{
		std::filesystem::rename(szSource, szRename);
	}

	static uintmax_t file_size(const std::string& szFilePath)
	{
		return std::filesystem::file_size(szFilePath);
	}

	static bool exists(const std::string& szFilePath)
	{
		return std::filesystem::exists(szFilePath);
	}
};


FileHandle::FileHandle()
{
}


FileHandle::~FileHandle()
{
}

void FileHandle::copyFile(const std::string& source, const std::string& dest) {
	const auto copyOptions = std::filesystem::copy_options::update_existing
		| std::filesystem::copy_options::recursive;
	std::filesystem::copy(source, dest, copyOptions);
}

bool FileHandle::removeAll(const std::string& source) {
	return std::filesystem::remove_all(source);
}


std::vector<std::string> FileHandle::readFile(const std::string& filepath) {
	std::vector<std::string> fileData;
	std::ifstream f(filepath);
	if (f.is_open()) {
		std::string line;
		while (std::getline(f, line)) {
			fileData.push_back(line);
		}
		f.close();
		return fileData;
	}
	else {
		throw io_exception("unable to perform io operation on file '" + filepath + "'; file not found or unable to open");
	}
}

std::string FileHandle::readLine(const std::string& filepath, unsigned int lineNumber) {
	std::string fileData;
	std::ifstream f(filepath);
	if (f.is_open()) {
		std::string line;
		unsigned int counter = 0;
		while (std::getline(f, line)) {
			if (counter == lineNumber) {
				fileData = line;
				break;
			}
			counter++;
		}
		f.close();
		return fileData;
	}
	else {
		throw io_exception("unable to perform io operation on file '" + filepath + "'; file not found or unable to open");
	}
}

unsigned int FileHandle::countLines(const std::string& filepath) {
	std::ifstream f(filepath);
	if (f.is_open()) {
		std::string line;
		unsigned int counter = 0;
		while (std::getline(f, line)) {
			counter++;
		}
		f.close();
		return counter;
	}
	else {
		throw io_exception("unable to perform io operation on file '" + filepath + "'; file not found or unable to open");
	}
}

std::string FileHandle::readFileAsString(const std::string& filepath)
{
	std::string fileData{ "" };
	std::ifstream f(filepath);
	if (f.is_open()) {
		std::string line;
		while (std::getline(f, line)) {
			fileData += line + "\n";
		}
		f.close();
		return fileData;
	}
	else {
		throw io_exception("unable to perform io operation on file '" + filepath + "'; file not found or unable to open");
	}
}


void FileHandle::writeToFile(const std::string& filepath, std::vector<std::string> data) {
	std::ofstream f(filepath);
	if (f.is_open()) {
		for (unsigned int i{ 0 }; i < data.size(); i++) {
			f << data[i] << "\n";
		}
		f.close();
		return;
	}
	else {
		throw io_exception("unable to perform io operation on file '" + filepath + "'; file not found or unable to open");
	}
}


std::vector<std::string> FileHandle::split(std::string line, char splitOn) {
	std::vector<std::string> newString;
	std::vector<int> indices;
	indices.push_back(-1);
	for (unsigned int i = 0; i < line.size(); i++) {
		if (line[i] == splitOn) {
			indices.push_back(i);
		}
	}//end for loop
	indices.push_back(line.size());//was indices size-1
	for (unsigned int j = 0; j < indices.size() - 1; j++) {

		std::string sub = line.substr(indices[j] + 1, indices[j + 1] - indices[j] - 1);
		//cout << "sub: "<<sub << endl;
		newString.push_back(sub);
	}//end for loop
	if (newString.size() == 0) {
		newString.push_back(line);
	}

	return newString;

}

std::vector<std::string> FileHandle::splitOnFirst(std::string str, char splitOn) {
	std::vector<std::string> newStr;
	for (unsigned int i{ 0 }; i < str.size(); i++) {
		if (str[i] == splitOn) {
			auto tempStr = str.substr(i, str.size() - 1);
			auto tempStr2 = str.substr(i);
			newStr.push_back(tempStr);
			newStr.push_back(tempStr2);
		}
	}
	if (newStr.size() == 0) {
		return { str };
	}
	else {
		return newStr;
	}
}


#endif