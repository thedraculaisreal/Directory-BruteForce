#include <iostream>
#include <curl/curl.h>
#include <Windows.h>

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output); 

int main()
{
	CURL* curl; // defining handle for libcurl.
	CURLcode res; // stores result to check if success.
	std::string response; // holds response.
	
	curl = curl_easy_init(); // intializes libcurl.

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://hyperion.ac/Account/Login"); // specifies url.

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback); // handling data recieved.
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); // writing data to response.

		res = curl_easy_perform(curl); // performs http request.

		if (res != CURLE_OK)
		{
			printf("Curl easy perform failed\n");
			return 1;
		}
		else
		{
			printf("Response: %s\n", response.c_str()); // have to add c_str to conver string to c style string.
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