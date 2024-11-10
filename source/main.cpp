#include <iostream>
#include <curl/curl.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <thread>

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output); 

int main(int argc, char* argv[])
{
	/*if (argc != 3)
	{
		printf("Usage: buster.exe <URL> <LIST PATH>");
		return 1;
	}*/
	
	std::string url{ "https://www.instagram.com/"}; // url.
	const char* list{ "C:\\Users\\schwarztoter\\Desktop\\big.txt"}; // wordlist path.
	std::vector<std::string> directories;
	std::vector<std::string> results;
	std::string buffer;

	std::ifstream inputFile(list);

	while (std::getline(inputFile, buffer))
	{
		directories.push_back(buffer);
	}

	CURL* curl; // defining handle for libcurl.
	CURLcode result; // stores result to check if success.
	std::string response; // holds response.
	
	curl = curl_easy_init(); // intializes libcurl.

	if (curl)
	{
		for (const auto& directory : directories)
		{
			std::string copyUrl = url;
			curl_easy_setopt(curl, CURLOPT_URL, copyUrl.append(directory).c_str()); // specifies url.
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback); // handling data recieved.
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); // writing data to response.

			result = curl_easy_perform(curl); // performs http request.

			if (result != CURLE_OK)
			{
				printf("Curl easy perform failed\n");
				return 1;
			}
			else
			{
				int responseCode;

				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

				if (responseCode != 200)
				{
					continue;
				}	

				printf("%s [Response:%d]\n", copyUrl.c_str(), responseCode); // have to add c_str to conver string to c style string.
				curl_easy_cleanup(curl); // cleanup and releasing of resources.
			}
		}

		curl_easy_cleanup(curl); // cleanup and releasing of resources.
	}

}

// processes the http response from server.
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
	output->append((char*)contents, size * nmemb); // appends the contents to the output.
	return size * nmemb;
}