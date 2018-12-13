//
// Created by Connor Thompson on 12/6/18.
//

#ifndef FINALPROJECT_USER_H
#define FINALPROJECT_USER_H

#include <iostream>
#include <string>
#include <cstring>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using namespace web::http::client;
using namespace web::http;
using json = nlohmann::json;

struct order {
    string orderType;
    json orderData;
    string orderLocation;
    vector<string> *sides;

    order() {
        orderType = "";
        orderLocation = "Farrand Grab 'N Go";
        orderData = nullptr;
        sides = new vector<string>;
    }
};

class User {
private:
    int accountId;
    std::vector<order> *pastOrders;
    order *currentOrder;
    int currentOrderId;
public:
    User(int accountIdToSet);
    ~User();
    bool setCurrentOrderId(int orderId);
    int getCurrentOrderId();
    bool setAccountId(int accId);
    int getAccountId();
    std::vector<order> *getPastOrders();
    bool setCurrentOrder(order *currentOrder);
    order *getCurrentOrder();
    bool makeSandwichOrder(string breadType, string meat, vector<string> toppings, string location);
    bool makeBurgerOrder(string bun, string cook, vector<string> toppings, bool special, string location);
    int sendOrder();
};


#endif //FINALPROJECT_USER_H
