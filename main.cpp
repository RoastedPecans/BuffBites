#include <iostream>
#include <string>
#include <cstring>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <nlohmann/json.hpp>
#include  "openssl/bio.h"
#include  "openssl/ssl.h"
#include  "openssl/err.h"
#include <vector>
#include "User.h"


using namespace std;
using namespace web::http::client;
using namespace web::http;
using json = nlohmann::json;

User *registerPost(string name, string email, string password, int studentId, string school) {
    cout << "Creating account" << endl;

    // Create JSON with the cooresponding data to include in POST request
    json registerPost;
    registerPost["name"] = name;
    registerPost["email"] = email;
    registerPost["password"] = password;
    registerPost["studentId"] = studentId;
    registerPost["school"] = school;
    registerPost["verification"] = "ZTLMRfyEknlL86jjJ02SLwed3rzoKT4RCCX5dBQaJE6GT2tzjSSap4iaChTI";

    // Create http_client and http_POST request
    http_client client("https://farrandorder.me/register");
    http_request requester;
    requester.set_method(methods::POST);
    requester.set_body(registerPost.dump());
    requester.headers().set_content_type(U("application/json"));

    // Send POST and get response from server
    auto response = client.request(requester);

    // Get body + status code of response
    auto body = response.get();
    int statusCode = body.status_code();

    // If status code does not indicate success
    if (statusCode < 200 || statusCode >= 300) {
        cout << "Problem creating account" << endl;
        return nullptr;
    }

    // Search the response body to get the account id returned
    string searchKey = "Account Id: ";

    // Find the start of the line "Account Id: xxx" returned in response
    int leftIndex = (int) body.to_string().rfind(searchKey);

    // Substring from the colon (:) to the end of the line to get the accountId
    int accountId = stoi(body.to_string().substr(leftIndex + searchKey.length()));

    User *newUser = new User(accountId);

    return newUser;
}

User *loginPost(string email, string password, string schoolName) {
    // Create JSON with the cooresponding data to include in POST request
    json loginPost;
    loginPost["email"] = email;
    loginPost["password"] = password;
    loginPost["school"] = schoolName;
    loginPost["verification"] = "c6jk0sBEegr4KMHLUNfRYc2XSWgUSKgZEIbUZpvyvP5MxGhcAZdFJhxy8T9b";

    // Create http_client and http_POST request
    http_client client("https://farrandorder.me/login");
    http_request requester;
    requester.set_method(methods::POST);
    requester.set_body(loginPost.dump());
    requester.headers().set_content_type(U("application/json"));

    // Send POST and get response from server
    auto response = client.request(requester);

    // Get body + status code of response
    auto body = response.get();
    int statusCode = body.status_code();

    // If status code does not indicate success
    if (statusCode < 200 || statusCode >= 300) {
        cout << statusCode << endl;
        cout << "Problem logging in, please try again!" << endl;
        return nullptr;
    }

    // Search the response body to get the account id returned
    string searchKey = "Account Id: ";

    // Find the start of the line "Account Id: xxx" returned in response
    int leftIndex = (int) body.to_string().rfind(searchKey);

    // Substring from the colon (:) to the end of the line to get the accountId
    int accountId = stoi(body.to_string().substr(leftIndex + searchKey.length()));

    cout << accountId << endl;

    User *newUser = new User(accountId);

    return newUser;
}

int main(int argc, char* argv[])
{
    // SSL Set-up
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();

    //User *user = registerPost("Connor Thompson", "coth8721@colorado.edu", "alexborland", 12345678, "University of Colorado Denver");
    User *user = loginPost("coth8721@colorado.edu", "alexborland", "University of Colorado Denver");

    cout << "Account " << user->getAccountId() << " logged in. " << endl << endl;

    // Create toppings that we want
    vector<string> toppings;
    toppings.emplace_back("lettuce");
    toppings.emplace_back("tomato");

    // Make order
    user->makeBurgerOrder("wheat", "medium-rare", toppings, true);
    cout << "Order Type: " << user->getCurrentOrder()->orderType << endl;
    cout << user->getCurrentOrder()->orderData.dump() << endl;

    cout << endl << "Sending order" << endl;
    user->sendOrder();
    cout << "Sent Order Id: " << user->getCurrentOrderId() << endl;

    return 0;
}