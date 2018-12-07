//
// Created by Connor Thompson on 12/6/18.
//

#include "User.h"

User::~User() = default;

bool User::setAccountId(int accId) {
    this->accountId = accId;
    return true;
};

int User::getAccountId() {
    return this->accountId;
}

std::vector<order> *User::getPastOrders() {
    return this->pastOrders;
}

bool User::setCurrentOrder(order *orderToSet) {
    this->currentOrder = orderToSet;
    return true;
}

order *User::getCurrentOrder() {
    return this->currentOrder;
}

bool User::setCurrentOrderId(int orderId) {
    this->currentOrderId = orderId;
    return true;
}

int User::getCurrentOrderId() {
    return currentOrderId;
}

User::User(int accountIdToSet) {
    setAccountId(accountIdToSet);
    setCurrentOrderId(0);
    this->currentOrder = new order();
    this->pastOrders = new vector<order>;
}

bool User::makeSandwichOrder(string breadType, string meat, vector<string> toppings) {
    cout << "making sandwich order" << endl << endl;
    json sandwichJSON = nullptr;
    sandwichJSON["orderType"]["breadType"] = breadType;
    sandwichJSON["meat"] = meat;

    if (!toppings.empty()) {
        int topping = 0;
        for (int i = 0; i < toppings.size(); i++) {
            sandwichJSON["toppings"]["topping" + to_string(topping)] = toppings.at(i);
            topping++;
        }
    }

    order *sandwichOrderToReturn = new order();

    sandwichOrderToReturn->orderType = "sandwich";
    sandwichOrderToReturn->orderData = sandwichJSON;

    currentOrder = sandwichOrderToReturn;

    return true;
}

bool User::makeBurgerOrder(string bun, string cook, vector<string> toppings, bool special) {
    cout << "making burger order" << endl << endl;
    json burgerJSON = nullptr;
    burgerJSON["bunType"] = bun;
    burgerJSON["cookType"] = cook;
    burgerJSON["special"] = special;

    if (!toppings.empty()) {
        int topping = 0;
        for (unsigned long i = 0; i < toppings.size(); i++) {
            burgerJSON["toppings"]["topping" + to_string(topping)] = toppings.at(i);
            topping++;
        }
    }

    order *burgerOrderToReturn = new order();

    burgerOrderToReturn->orderType = "burger";
    burgerOrderToReturn->orderData = burgerJSON;

    currentOrder = burgerOrderToReturn;

    return true;
}

int User::sendOrder() {
    // Create JSON with the cooresponding data to include in POST request
    json orderPost;
    orderPost["orderData"] = getCurrentOrder()->orderData;
    orderPost["orderType"] = getCurrentOrder()->orderType;
    orderPost["id"] = getAccountId();
    orderPost["verification"] = "rtkVM8uOpxqZVYBop5zNs8X6WS6pppMBHgd1CqxDZFXwqoXkkzY0jZaa0wcI";

    cout << orderPost.dump() << endl;

    // Create http_client and http_POST request
    http_client client("https://farrandorder.me/createOrder");
    http_request requester;
    requester.set_method(methods::POST);
    requester.set_body(orderPost.dump());
    requester.headers().set_content_type(U("application/json"));

    // Send POST and get response from server
    auto response = client.request(requester);

    // Get body + status code of response
    auto body = response.get();
    int statusCode = body.status_code();

    // If status code does not indicate success
    if (statusCode < 200 || statusCode >= 300) {
        cout << statusCode << endl;
        cout << body.to_string() << endl;
        cout << "Problem sending order, please try again!" << endl;
        return -1;
    }

    else {
        // Search the response body to get the account id returned
        string searchKey = "Order Id: ";

        // Find the start of the line "Account Id: xxx" returned in response
        int leftIndex = (int) body.to_string().rfind(searchKey);

        // Substring from the colon (:) to the end of the line to get the accountId
        int orderId = stoi(body.to_string().substr(leftIndex + searchKey.length()));


        currentOrderId = orderId;

        return orderId;
    }
}