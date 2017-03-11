/*
 *  C++ lib\Base64.h
 *  Lucas Barbosa | 2017 | Open source software
 */

#ifndef BASE_64_H_INCLUDED
#define BASE_64_H_INCLUDED

#include <iostream>
#include <vector>

using namespace std;

/*
 *  Base-64 is an enconding procedure, not an ecryption protocol
 *  unlike AES which is really the industry standard these days.
 *  This file includes base64 encoders and decoders as well as
 *  base64 encryption AND decryption. (decryption may be depricated
 *  in future releases).
 */

namespace Base64
{
  std::string base64_encode(const std::string &string_to_be_encoded);
  std::string base64_decode(const std::string &string_to_be_decoded);

  const std::string &salt_1 = "LM::TB::BB";
  const std::string &salt_2 = "_/_:_77:";
  const std::string &salt_3 = "line=wowcpptoday";

  std::string base64_encrypt(std::string string_to_be_encrypted)
  {
    string_to_be_encrypted = salt_1 + string_to_be_encrypted + salt_2 + salt_3;
    string_to_be_encrypted = base64_encode(string_to_be_encrypted);
    string_to_be_encrypted.insert (7, salt_3);
    string_to_be_encrypted += salt_1;
    string_to_be_encrypted = base64_encode(string_to_be_encrypted);
    string_to_be_encrypted = salt_2 + salt_3 + string_to_be_encrypted + salt_1;
    string_to_be_encrypted = base64_encode(string_to_be_encrypted);
    string_to_be_encrypted.insert (1, "L");
    string_to_be_encrypted.insert (7, "M");

    std::string encrypted_string(string_to_be_encrypted);
    return encrypted_string;
  }

  std::string base64_decrypt(std::string string_to_be_decrypted)
  {
    string_to_be_decrypted = string_to_be_decrypted.erase (7, 1);
    string_to_be_decrypted = string_to_be_decrypted.erase (1, 1);
    string_to_be_decrypted = base64_decode(string_to_be_decrypted);
    string_to_be_decrypted = string_to_be_decrypted.substr (salt_2.length() + salt_3.length());
    string_to_be_decrypted = string_to_be_decrypted.substr (0, string_to_be_decrypted.length() - salt_1.length());
    string_to_be_decrypted = base64_decode(string_to_be_decrypted);
    string_to_be_decrypted = string_to_be_decrypted.substr (0, string_to_be_decrypted.length() - salt_1.length());
    string_to_be_decrypted = string_to_be_decrypted.erase (7, salt_3.length());
    string_to_be_decrypted = base64_decode(string_to_be_decrypted);
    string_to_be_decrypted = string_to_be_decrypted.substr (salt_1.length());
    string_to_be_decrypted = string_to_be_decrypted.substr (0, string_to_be_decrypted.length() - salt_2.length() - salt_3.length());

    std::string decrypted_string(string_to_be_decrypted);
    return decrypted_string;
  }

  const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  std::string base64_encode(const std::string &string_to_be_encoded)
  {
    std::string output_string;
    // index used to map values to Base64 table
    int val = 0;
    // bits will be used to represent number of bits in a Base64 sub-group (groups of 6)
    int bits = -6;
    const unsigned int base63 = 0x3F; // hex-value for 63
    // processing string: string_to_be_encoded, c will be assigned every character from string_to_be_encoded
    for(const auto &c : string_to_be_encoded)
    {
      // <binary_shifting> '<<' is a binary operator, which just means that the resulting value will be shifted LEFT (<<) 8 places
      val = (val << 8) + c;
      bits += 8;
      while(bits >= 0)
      {
        output_string.push_back(BASE64_CODES[(val >> bits) & base63]);
        // we decrease the number of bits by 6, since we are grouping by 6 bits
        bits -= 6;
      }
    }
    if (bits > -6)
    {
      output_string.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & base63]);
    }
    while(output_string.size() % 4)
    {
      output_string.push_back('=');
    }

    std::string encoded_string = output_string;
    return encoded_string;
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

}// end of namespace Base64

#endif // BASE_64_H_INCLUDED
