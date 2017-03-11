/*
 *  C++ Base 64 decryption protocol
 *  Lucas Barbosa | 2017 | Open source software
 */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const std::string &salt_1 = "LM::TB::BB";
const std::string &salt_2 = "_/_:_77:";
const std::string &salt_3 = "line=wowcpptoday";

const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_decrypt(std::string string_to_be_decrypted);
std::string base64_decode(const std::string &string_to_be_decoded);

int main(int argc, char *argv[])
{
	
	if (argc != 3)
	{
		return std::cout << "Program needs two arguments: input and output." << std::endl, 2;
	}
	
	std::string arg_input(argv[1]), arg_output(argv[2]);
	std::ifstream input_file(arg_input);
	if (!input_file)
	{
		return std::cout << "Cannot read input file: '" << arg_input << "'" << std::endl, 3;
	}
	std::string file_data;
	input_file >> file_data;
	if (!input_file)
	{
		return std::cout << "Input file: '" << arg_input << "' is corrupted" << std::endl, 4;
	}
	
	file_data = base64_decrypt(file_data);
	
	std::ofstream output_file(arg_output, std::ios::in | std::ios::app);
	if (!output_file)
	{
		return std::cout << "Cannot write to output file '" << arg_output << "'" << std::endl, 5;
	}
	// decrypted string information into output file
	output_file << file_data;
	output_file.close();
	
	std::cout << "Decoding was successfull" << std::endl;
	
	return 0;	
	
}

std::string base64_decrypt(std::string string_to_be_decrypted)
{
  string_to_be_decrypted = string_to_be_decrypted.erase(7, 1);
  string_to_be_decrypted = string_to_be_decrypted.erase(1, 1);
  string_to_be_decrypted = base64_decode(string_to_be_decrypted);
  string_to_be_decrypted = string_to_be_decrypted.substr(salt_2.length() + salt_3.length());
  string_to_be_decrypted = string_to_be_decrypted.substr(0, string_to_be_decrypted.length() - salt_1.length());
  string_to_be_decrypted = base64_decode(string_to_be_decrypted);
  string_to_be_decrypted = string_to_be_decrypted.substr(0, string_to_be_decrypted.length() - salt_1.length());
  string_to_be_decrypted = string_to_be_decrypted.erase(7, salt_3.length());
  string_to_be_decrypted = base64_decode(string_to_be_decrypted);
  string_to_be_decrypted = string_to_be_decrypted.substr(salt_1.length());
  string_to_be_decrypted = string_to_be_decrypted.substr(0, string_to_be_decrypted.length() - salt_2.length() - salt_3.length());
  std::string decrypted_string(string_to_be_decrypted);
  return decrypted_string;
}

std::string base64_decode(const std::string &string_to_be_decoded)
{
  std::string decoded_string;
  std::vector<int> vec(256, -1);
  for (int i = 0; i < 64; i++)
    vec [BASE64_CODES[i]] = i;
    int val = 0, bits = -8;

  for (const auto &i : string_to_be_decoded)
  {
    if (vec[i] == -1)
    {
      break;
    }

    val = (val << 6) + vec[i];
    bits += 6;

    if (bits >= 0)
    {
      decoded_string.push_back(char((val >> bits) & 0xFF));
      bits -= 8;
    }
  }

  return decoded_string;
}
