#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using byte_t = char;

int encode(const std::string &path, byte_t offset)
{
	std::ifstream in = std::ifstream(path, std::ios::in | std::ios::binary);
	std::ofstream out;
	byte_t byte = 0;
	if (!in)
	{
		std::cerr << "Failed to open the input file: \""
				  << path << "\" or it's empty." << std::endl;
		goto FAIL_RET;
	}
	out.open(path + "-encode_res.out", std::ios::out | std::ios::binary);
	if (!out)
	{
		std::cerr << "Failed to open the output file: \""
				  << path << "\"." << std::endl;
		goto FAIL_RET;
	}
	while (in.read(&byte, 1))
	{
		out.write(&(byte += offset), 1);
	}
	return 1;
FAIL_RET:
	return 0;
}

int main(int argc, char *argv[])
{
	std::ios::sync_with_stdio(false);
	int offset;
	std::string path;
	std::stringstream ss;
	if (argc > 2)
	{
		for (int i = 1; i + 1 < argc; i += 2)
		{
			ss << argv[i];
			if (!(ss >> offset))
			{
				std::cerr << "Invalid parameter for the offset!" << std::endl;
				while (ss)
				{
					byte_t byte;
					ss.read(&byte, 1);
				} // clear the buffer
				ss.clear();
				continue;
			}
			if (encode(argv[i + 1], offset % 128) == 1)
			{
				std::cout << "Success." << std::endl;
			}
			else
			{
				std::cerr << "Failure." << std::endl;
			}
		}
	}
	else
	{
		while (std::cin)
		{
			std::cout << "Input the offset of each byte: ";
			while (!(std::cin >> offset))
			{
				if (std::cin.eof())
				{
					std::cout << "Exit." << std::endl;
					goto NORMAL_RET;
				}
				std::cerr << "Invalid input of the offset!" << std::endl;
				std::cout << "Input the offset of each byte: ";
			}
			std::getline(std::cin, path); // end-line
			std::cout << "Input the path of the file to encode: ";
			if (!std::getline(std::cin, path))
			{
				if (std::cin.eof())
				{
					std::cout << "Exit." << std::endl;
					goto NORMAL_RET;
				}
				std::cerr << "Fatal error: Failed to write data into the string!" << std::endl;
				goto ERR_RET;
			}
			if (encode(path, offset % 128) == 1)
			{
				std::cout << "Success." << std::endl;
			}
			else
			{
				std::cerr << "Failure." << std::endl;
			}
		}
	}
NORMAL_RET:
	return 0;
ERR_RET:
	return -1;
}