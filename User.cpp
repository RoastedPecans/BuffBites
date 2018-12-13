//
// Created by Connor Thompson on 12/6/18.
//

#include "User.h"

User::~User() = default;

// Set the user account id (primary key id from DB)
bool User::setAccountId(int accId) {
    this->accountId = accId;
    return true;
};

// Get account id of logged in user
int User::getAccountId() {
    return this->accountId;
}

// Get past orders for user
std::vector<order> *User::getPastOrders() {
    return this->pastOrders;
}

// Set the current order for the user (deprecated)
bool User::setCurrentOrder(order *orderToSet) {
    this->currentOrder = orderToSet;
    return true;
}

// Get current order for user
order *User::getCurrentOrder() {
    return this->currentOrder;
}

// Set the current order for the user
bool User::setCurrentOrderId(int orderId) {
    this->currentOrderId = orderId;
    return true;
}

// Get the current order ID (not used)
int User::getCurrentOrderId() {
    return currentOrderId;
}

// Create new account with cooresponding PKID from DB
User::User(int accountIdToSet) {
    setAccountId(accountIdToSet);
    setCurrentOrderId(0);
    this->currentOrder = new order();
    this->pastOrders = new vector<order>;
}

// Make sandwich order (not used as of right now)
bool User::makeSandwichOrder(string breadType, string meat, vector<string> toppings, string location) {
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
    sandwichOrderToReturn->orderLocation = location;

    currentOrder = sandwichOrderToReturn;

    return true;
}

// Make burger order according to data passed in
bool User::makeBurgerOrder(string bun, string cook, vector<string> toppings, bool special, string location) {
    // Make JSON
    json burgerJSON = nullptr;
    burgerJSON["bunType"] = bun;
    burgerJSON["cookType"] = cook;
    burgerJSON["special"] = special;

    // Add toppings
    if (!toppings.empty()) {
        int topping = 0;
        for (unsigned long i = 0; i < toppings.size(); i++) {
            burgerJSON["toppings"]["topping" + to_string(topping)] = toppings.at(i);
            topping++;
        }
    }

    // Create new order
    order *burgerOrderToReturn = new order();

    // Set attributes
    burgerOrderToReturn->orderType = "burger";
    burgerOrderToReturn->orderData = burgerJSON;
    burgerOrderToReturn->orderLocation = location;

    currentOrder = burgerOrderToReturn;

    return true;
}

// Send the order to the webserver via POST request
int User::sendOrder() {
    // Create JSON with the cooresponding data to include in POST request
    json orderPost;
    orderPost["orderData"] = getCurrentOrder()->orderData;
    orderPost["orderType"] = getCurrentOrder()->orderType;
    orderPost["orderLocation"] = getCurrentOrder()->orderLocation;
    orderPost["id"] = getAccountId();
    orderPost["verification"] = "rtkVM8uOpxqZVYBop5zNs8X6WS6pppMBHgd1CqxDZFXwqoXkkzY0jZaa0wcI";

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

        // Set the current order id to be accurate
        currentOrderId = orderId;

        return orderId;
    }
}