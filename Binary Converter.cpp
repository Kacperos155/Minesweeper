#include "Binary Converter.hpp"
using namespace std;

Binary_Converter::Binary_Converter(std::string binary_name)
	:input_name(binary_name), output_name(""), size(-1), binary_file_size(-1), pointer(0)
{
	MeasureBinarySize();
}

void Binary_Converter::MeasureBinarySize()
{
	ifstream m_file(input_name, ios::binary);
	m_file.seekg(0, m_file.end);
	if (m_file.tellg() < 0)
		binary_file_size = 0;
	else
		binary_file_size = static_cast<size_t>(m_file.tellg());
}

void Binary_Converter::Summary()
{
	MeasureBinarySize();
	cout << "Loaded file < " << output_name << " >\n";
	cout << "\tLoaded size < " << size << " > bytes \n";
	cout << "\tPointer at position < " << pointer << " > \n";
	cout << "Binary file < " << input_name << " >\n";
	cout << "\tSize of binary file < " << binary_file_size << " > bytes \n";
	cout << "\n";
}

void Binary_Converter::BufferWork(bool save)
{
	if (save)
	{
		file.write(reinterpret_cast<char*>(buffer.data()), size);
		buffer.clear();
	}
	else
	{
		buffer.clear();
		buffer.resize(size);
		file.read(reinterpret_cast<char*>(buffer.data()), size);
		pointer += size;
	}
}

void Binary_Converter::LoadFromFile(string input_name)
{
	file.open(input_name, ios::in | ios::binary);
	this->output_name = input_name;

	file.seekg(0, file.end);
	size = static_cast<int>(file.tellg());
	file.seekg(0, file.beg);

	BufferWork(false);
	file.close();
}

void Binary_Converter::LoadFromBinary()
{
	file.open(input_name, ios::in | ios::binary);
	char c_name[65];

	file.seekg(pointer);
	file.read(c_name, 64);
	output_name = c_name;

	char c_size[9];
	file.read(c_size, 8);
	size = stoi(c_size);
	pointer += 72;

	BufferWork(false);
	file.close();
}

void Binary_Converter::SaveToFile()
{
	string location = "Output/";
	location += output_name;
	file.open(location, ios::out | ios::binary);
	if (!file.is_open())
	{
		file.close();
		file.open(output_name, ios::out | ios::binary);
	}
	BufferWork(true);
	file.close();
}

void Binary_Converter::SaveToBinary()
{
	file.open(input_name, ios::out | ios::binary | ios::app);
	char c_name[65];
	strcpy_s(c_name, 64, output_name.c_str());
	file.write(c_name, 64);

	char c_size[9];
	strcpy_s(c_size, 8, to_string(size).c_str());
	file.write(c_size, 8);

	BufferWork(true);
	file.close();
}

void Binary_Converter::ImportBuffer(std::vector<char> _buffer, std::size_t _size, std::string _output_name)
{
	buffer = _buffer;
	size = _size;
	output_name = _output_name;
}

std::vector<char> Binary_Converter::ExportBuffer() const
{
	return buffer;
}

std::string Binary_Converter::GetName() const
{
	return output_name;
}

bool Binary_Converter::Is_Empty() const
{
	return (pointer >= binary_file_size);
}

bool Binary_Converter::Is_Good() 
{
	MeasureBinarySize();
	if (binary_file_size > 0)
		return true;
	return false;
}
