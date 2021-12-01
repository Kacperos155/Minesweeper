//By Kacper Zielinski
#pragma once
#ifndef Konwenter_Binarny_HPP
#define Konwenter_Binarny_HPP
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>
#include <memory>

class Binary_Converter
{
public:
	Binary_Converter(std::string binary_name);
	void Summary();
	void LoadFromFile(std::string input_name);
	void LoadFromBinary();
	void SaveToFile();
	void SaveToBinary();
	void ImportBuffer(std::vector<char> buffer, std::size_t size, std::string output_name);
	std::vector<char> ExportBuffer() const;
	std::string GetName() const;
	bool Is_Empty() const;
	bool Is_Good();

private:
	std::fstream file;
	std::string input_name;
	std::string output_name;
	std::size_t size;
	std::size_t binary_file_size;
	std::size_t pointer;
	std::vector<char> buffer;
	void MeasureBinarySize();
	void BufferWork(bool save);
};

#endif Konwenter_Binarny_HPP