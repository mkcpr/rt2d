#include <curl/curl.h>

#include <json/json.hpp>
using json = nlohmann::json;

struct MemoryStruct {
	char *memory;
	size_t size;
};

size_t my_dummy_write(char *contents, size_t size, size_t nmemb, void *userdata) {
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userdata;

	char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

void testCurl() {

	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (curl) {
		//curl_easy_setopt(curl, CURLOPT_URL, "http://jsonplaceholder.typicode.com/users/1");
		//curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/todos/1");
		//curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/todos");
		curl_easy_setopt(curl, CURLOPT_URL, "https://portfoliostudenten.nl/MCU/json/users.php");
		//curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.nl");


		struct MemoryStruct chunk;
		chunk.memory = (char*)malloc(1);	/* will be grown as needed by the realloc above */
		chunk.size = 0;						/* no data at this point */

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &my_dummy_write);

		char errbuf[CURL_ERROR_SIZE];
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);	/* provide a buffer to store errors in */
		errbuf[0] = 0;											/* set the error buffer as empty before performing a request */

		curl_easy_setopt(curl, CURLOPT_CERTINFO, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);

		if (!res) {
			struct curl_certinfo *certinfo;

			res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);

			/*
			if (!res && certinfo) {
				int i;

				printf("%d certs!\n", certinfo->num_of_certs);

				for (i = 0; i < certinfo->num_of_certs; i++) {
					struct curl_slist *slist;

					for (slist = certinfo->certinfo[i]; slist; slist = slist->next)
						printf("%s\n", slist->data);

				}
			}
			*/

		}


		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		else {
			printf("%lu bytes retrieved\n", (unsigned long)chunk.size);


			std::string data(chunk.memory);

			try {
				json jsonData = json::parse(data);


				//Show Users from: https://portfoliostudenten.nl/MCU/json/users.php
				//std::cout << jsonData["users"] << "\n";
				if (jsonData["users"].is_array()) {
					jsonData = jsonData["users"];
					for (json::iterator it = std::begin(jsonData); it != std::end(jsonData); ++it) {
						json user = *it;
						if (user.is_object()) {
							for (json::iterator it2 = user.begin(); it2 != user.end(); ++it2) {
								std::cout << "Key: " << it2.key() << " - Value: " << it2.value() << "\n";
							}
						}
					}
				}


				//std::cout << jsonData["id"];
				//std::cout << jsonData["address"]["street"];
				//std::cout << jsonData["users"] << "\n";


				json::iterator it_begin;
				json::iterator it_end;

				if (jsonData.is_array()) {
					std::cout << "This is an array of size: " << jsonData.size() << "\n";

					it_begin = std::begin(jsonData);
					it_end = std::end(jsonData);
				}
				else if (jsonData.is_object()) {
					std::cout << "This is an object of size: " << jsonData.size() << "\n";

					it_begin = jsonData.begin();
					it_end = jsonData.end();
				}


				for (json::iterator it = it_begin; it != it_end; ++it) {
					json data = *it;

					if (data.is_array()) {
						std::cout << "arrayyyy";
					}
					else if (data.is_object()) {
						for (json::iterator itt = data.begin(); itt != data.end(); ++itt) {
							std::cout << "Key: " << itt.key() << " - Value: " << itt.value() << "\n";
						}
						std::cout << "\n";
					}
					else {
						std::cout << it.value() << "\n";
						//std::cout << *it;
					}
				}
			}
			catch (json::exception& e) {
				// output exception information
				std::cout << "message: " << e.what() << '\n' << "exception id: " << e.id << std::endl;
			}
		}

		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

}