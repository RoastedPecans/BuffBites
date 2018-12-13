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
#include <stdio.h>
//#include "imgui.h"
//#include <GLFW/glfw3.h>
//#include "imgui_widgets.cpp"

using namespace std;
using namespace web::http::client;
using namespace web::http;
using json = nlohmann::json;

User *registerPost(string name, string email, string password, int studentId, string school) {
    cout << "Creating account" << endl;

    string lowercaseEmail;
    // Convert email to lowercase
    for (int i = 0; i < email.length(); i++) {
        lowercaseEmail += tolower(email[i]);
    }

    // Create JSON with the cooresponding data to include in POST request
    json registerPost;
    registerPost["name"] = name;
    registerPost["email"] = lowercaseEmail;
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
    string lowercaseEmail;
    // Convert email to lowercase
    for (int i = 0; i < email.length(); i++) {
        lowercaseEmail += tolower(email[i]);
    }
    // Create JSON with the cooresponding data to include in POST request
    json loginPost;
    loginPost["email"] = lowercaseEmail;
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

    User *newUser = new User(accountId);

    return newUser;
}

int main(int argc, char* argv[])
{
    // SSL Set-up
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();

    // Variables for logging in/registering
    string name;
    string email;
    string password;
    string school;
    string choice;
    int schoolId = 0;
    string schoolIdString;
    User *user;

    // Loop used to login existing user/register new user
    while (true) {
        cout << "Would you like to login or register?" << endl;
        getline(cin, choice);

        // Login
        if (tolower(choice[0]) == 'l') {
            cout << "Please enter your email: ";
            getline(cin, email);
            cout << endl << "Please enter your password: ";
            getline(cin, password);
            cout << endl << "Please enter your school: ";
            getline(cin, school);
            cout << "Attempting to login..." << endl;

            // Send Login POST request
            user = loginPost(email, password, school);
            if (user != nullptr and user->getAccountId() != 0) {
                cout << "Logged in!" << endl;
                break;
            }
            else {
                cout << "Unable to login, please try again!" << endl;
                return -1;
            }
        }

        // Register
        else {
            cout << "Registering new user" << endl;
            cout << "Please enter your name: ";
            getline(cin, name);
            cout << endl << "Please enter your email: ";
            getline(cin, email);
            cout << endl << "Please enter your password: ";
            getline(cin, password);
            cout << endl << "Please enter your school: ";
            getline(cin, school);
            cout << endl << "Please enter your student id number: ";
            getline(cin, schoolIdString);
            schoolId = stoi(schoolIdString);
            cout << "Attempting to register user..." << endl;

            // Send register POST request
            user = registerPost(name, email, password, schoolId, school);
            if (user != nullptr and user->getAccountId() != 0) {
                cout << "Registered and logged in!" << endl;
                break;
            }
            else {
                cout << "Unable to register, please try again!" << endl;
                return -1;
            }
        }
    }

    // Variables used to make burger order
    vector<string> toppings;
    string cookType;
    string breadType;
    bool special = false;
    string specialString;
    string location = "Farrand Grab N' Go";
    string topping;

    // Loop used for user input into burger order
    while (true) {
        cout << "Now getting information for burger order: " << endl;
        cout << "Would you like to get the daily special? (y/n): ";
        getline(cin, specialString);
        if (tolower(specialString[0]) == 'y') {
            special = true;
        }
        cout << "Please enter what bread type you'd like (wheat, white, veggie, none): ";
        getline(cin, breadType);
        cout << "Please enter how long you'd like your burger cooked (rare, medium-rare, medium, medium-well, well): ";
        getline(cin, cookType);
        cout << "Now we will add your toppings, please enter 'Done' when done. Options are tomato, onion, pickle, lettuce." << endl;
        int counter = 1;
        while (true) {
            cout << "Topping " << to_string(counter) << ": ";
            getline(cin, topping);
            if (tolower(topping[0]) == 't') {
                toppings.emplace_back("tomato");
            }
            if (tolower(topping[0]) == 'o') {
                toppings.emplace_back("onion");
            }
            if (tolower(topping[0]) == 'p') {
                toppings.emplace_back("pickle");
            }
            if (tolower(topping[0]) == 'l') {
                toppings.emplace_back("lettuce");
            }
            if (tolower(topping[0]) == 'd') {
                break;
            }
            counter++;
        }
        cout << "Sending order to Farrand Grab N' Go" << endl;

        // Make burger order object in C++
        user->makeBurgerOrder(breadType, cookType, toppings, special, location);

        // Send order to website with POST requestq
        int orderId = user->sendOrder();

        if (orderId > 0) {
            cout << "Order " << to_string(orderId) << " sent successfully! You can monitor its status online at https://farrandOrder.me/viewOrders" << endl;
            break;
        }
        else {
            cout << "Problem sending order, please try again." << endl;
            return -1;
        }


    }
    delete user;

    //User *user = loginPost("coth8721@colorado.edu", "Testing123!", "University of Colorado Boulder");
    //User *user = registerPost("Connor Thompson", "coth8721@colorado.edu", "Testing123!", 107554044, "University of Colorado Boulder");
    return 0;
}