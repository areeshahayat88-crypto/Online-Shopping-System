#include <iostream>
#include <fstream>
#include <string>
using namespace std;


// 1. STRUCTURES

struct Product
{
    int id;
    string name;
    string category;
    float price;
    int stock;
    Product* next;

    Product()
    {
        next = NULL;
    }
};

struct Cart
{
    int id;
    string name;
    float price;
    int quantity;
    Cart* next;

    Cart()
    {
        next = NULL;
    }
};

struct Order
{
    int orderID;
    int numberOfProducts;
    float total;
    Order* next;

    Order()
    {
        next = NULL;
    }
};

// Linked list for products
struct ProductList
{
    Product* head;
    Product* tail;

    ProductList()
    {
        head = NULL;
        tail = NULL;
    }
};

// Linked list for shopping cart
struct CartList
{
    Cart* head;
    Cart* tail;

    CartList()
    {
        head = NULL;
        tail = NULL;
    }
};

// Queue for orders + linked list for completed orders
struct OrderQueue
{
    Order* front;
    Order* rear;

    Order* completedHead;
    Order* completedTail;

    int nextOrderID;

    OrderQueue()
    {
        front = NULL;
        rear = NULL;

        completedHead = NULL;
        completedTail = NULL;

        nextOrderID = 1;
    }
};



// 2. PRODUCT SEARCH BY ID


Product* findProduct(ProductList& products, int id)
{
    Product* temp = products.head;

    while (temp != NULL)
    {
        if (temp->id == id)
        {
            return temp;
        }

        temp = temp->next;
    }

    return NULL;
}


// 3. FILE HANDLING: SAVE PRODUCTS


void saveProducts(ProductList& products)
{
    ofstream file("products.txt");

    Product* temp = products.head;

    while (temp != NULL)
    {
        file << temp->id << " "
             << temp->name << " "
             << temp->category << " "
             << temp->price << " "
             << temp->stock << endl;

        temp = temp->next;
    }

    file.close();
}



// 4. FILE HANDLING: SAVE INVENTORY


void saveInventory(ProductList& products)
{
    ofstream file("inventory.txt");

    Product* temp = products.head;

    while (temp != NULL)
    {
        file << temp->id << " "
             << temp->stock << endl;

        temp = temp->next;
    }

    file.close();
}



// 5. ADD PRODUCT


void addProduct(ProductList& products)
{
    Product* newProduct = new Product;

    cout << "\nEnter Product ID: ";
    cin >> newProduct->id;

    if (findProduct(products, newProduct->id) != NULL)
    {
        cout << "Product ID already exists!\n";
        delete newProduct;
        return;
    }

    cout << "Enter Product Name: ";
    cin >> newProduct->name;

    cout << "Enter Category: ";
    cin >> newProduct->category;

    cout << "Enter Price: ";
    cin >> newProduct->price;

    cout << "Enter Stock Quantity: ";
    cin >> newProduct->stock;

    if (newProduct->price < 0 || newProduct->stock < 0)
    {
        cout << "Price and stock cannot be negative!\n";
        delete newProduct;
        return;
    }

    newProduct->next = NULL;

    if (products.head == NULL)
    {
        products.head = newProduct;
        products.tail = newProduct;
    }
    else
    {
        products.tail->next = newProduct;
        products.tail = newProduct;
    }

    saveProducts(products);
    saveInventory(products);

    cout << "Product added successfully!\n";
}



// 6. DISPLAY PRODUCTS


void displayProducts(ProductList& products)
{
    if (products.head == NULL)
    {
        cout << "\nNo products available!\n";
        return;
    }

    Product* temp = products.head;

    cout << "\nID\tName\t\tCategory\tPrice\tStock\n";

    while (temp != NULL)
    {
        cout << temp->id << "\t"
             << temp->name << "\t\t"
             << temp->category << "\t\t"
             << temp->price << "\t"
             << temp->stock << endl;

        temp = temp->next;
    }
}



// 7. UPDATE PRODUCT


void updateProduct(ProductList& products)
{
    int id;

    cout << "\nEnter Product ID to update: ";
    cin >> id;

    Product* temp = findProduct(products, id);

    if (temp == NULL)
    {
        cout << "Product not found!\n";
        return;
    }

    cout << "Enter New Name: ";
    cin >> temp->name;

    cout << "Enter New Category: ";
    cin >> temp->category;

    cout << "Enter New Price: ";
    cin >> temp->price;

    cout << "Enter New Stock: ";
    cin >> temp->stock;

    if (temp->price < 0 || temp->stock < 0)
    {
        cout << "Invalid price or stock!\n";
        return;
    }

    saveProducts(products);
    saveInventory(products);

    cout << "Product updated successfully!\n";
}



// 8. DELETE PRODUCT


void deleteProduct(ProductList& products, CartList& cart)
{
    int id;

    cout << "\nEnter Product ID to delete: ";
    cin >> id;

    // Check whether product is in cart
    Cart* c = cart.head;

    while (c != NULL)
    {
        if (c->id == id)
        {
            cout << "Remove this product from cart first!\n";
            return;
        }

        c = c->next;
    }

    Product* temp = products.head;
    Product* previous = NULL;

    while (temp != NULL && temp->id != id)
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        cout << "Product not found!\n";
        return;
    }

    if (temp == products.head)
    {
        products.head = products.head->next;
    }
    else
    {
        previous->next = temp->next;
    }

    if (temp == products.tail)
    {
        products.tail = previous;
    }

    delete temp;

    if (products.head == NULL)
    {
        products.tail = NULL;
    }

    saveProducts(products);
    saveInventory(products);

    cout << "Product deleted successfully!\n";
}



// 9. LINEAR SEARCH


void searchProducts(ProductList& products)
{
    if (products.head == NULL)
    {
        cout << "\nNo products available to search!\n";
        return;
    }

    int choice;
    int id;
    float price;
    string value;

    cout << "\nSearch Product By:\n";
    cout << "1. Product ID\n";
    cout << "2. Product Name\n";
    cout << "3. Category\n";
    cout << "4. Price\n";
    cout << "Enter choice: ";
    cin >> choice;

    Product* temp = products.head;
    bool found = false;

    if (choice == 1)
    {
        cout << "Enter ID: ";
        cin >> id;
    }
    else if (choice == 2 || choice == 3)
    {
        cout << "Enter Name or Category: ";
        cin >> value;
    }
    else if (choice == 4)
    {
        cout << "Enter Price: ";
        cin >> price;
    }
    else
    {
        cout << "Invalid choice!\n";
        return;
    }

    while (temp != NULL)
    {
        if ((choice == 1 && temp->id == id) ||
            (choice == 2 && temp->name == value) ||
            (choice == 3 && temp->category == value) ||
            (choice == 4 && temp->price == price))
        {
            cout << "\nProduct Found!\n";
            cout << "ID: " << temp->id << endl;
            cout << "Name: " << temp->name << endl;
            cout << "Category: " << temp->category << endl;
            cout << "Price: " << temp->price << endl;
            cout << "Stock: " << temp->stock << endl;

            found = true;
        }

        temp = temp->next;
    }

    if (found == false)
    {
        cout << "Product not found!\n";
    }
}



// 10. CHECK WHICH PRODUCT COMES FIRST FOR BUBBLE SORT


bool shouldSwap(Product* a, Product* b, int choice)
{
    if (choice == 1)
        return a->id > b->id;

    else if (choice == 2)
        return a->name > b->name;

    else if (choice == 3)
        return a->category > b->category;

    else
        return a->price > b->price;
}


// 11. BUBBLE SORT


void sortProducts(ProductList& products)
{
    if (products.head == NULL || products.head->next == NULL)
    {
        cout << "\nNot enough products to sort!\n";
        return;
    }

    int choice;

    cout << "\nSort Products By:\n";
    cout << "1. Product ID\n";
    cout << "2. Product Name\n";
    cout << "3. Category\n";
    cout << "4. Price\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice < 1 || choice > 4)
    {
        cout << "Invalid choice!\n";
        return;
    }

    for (int i = 0; i < 1000; i++)
    {
        bool swapped = false;

        Product* temp = products.head;

        while (temp->next != NULL)
        {
            if (shouldSwap(temp, temp->next, choice))
            {
                // Swap product information
                int id = temp->id;
                temp->id = temp->next->id;
                temp->next->id = id;

                string name = temp->name;
                temp->name = temp->next->name;
                temp->next->name = name;

                string category = temp->category;
                temp->category = temp->next->category;
                temp->next->category = category;

                float price = temp->price;
                temp->price = temp->next->price;
                temp->next->price = price;

                int stock = temp->stock;
                temp->stock = temp->next->stock;
                temp->next->stock = stock;

                swapped = true;
            }

            temp = temp->next;
        }

        if (swapped == false)
        {
            break;
        }
    }

    saveProducts(products);
    saveInventory(products);

    cout << "Products sorted successfully!\n";
    displayProducts(products);
}



// 12. ADD PRODUCT TO CART


void addToCart(ProductList& products, CartList& cart)
{
    int id;
    int quantity;

    cout << "\nEnter Product ID: ";
    cin >> id;

    Product* p = findProduct(products, id);

    if (p == NULL)
    {
        cout << "Product is unavailable!\n";
        return;
    }

    cout << "Enter Quantity: ";
    cin >> quantity;

    if (quantity <= 0)
    {
        cout << "Invalid quantity!\n";
        return;
    }

    Cart* temp = cart.head;

    while (temp != NULL)
    {
        if (temp->id == id)
        {
            if (temp->quantity + quantity > p->stock)
            {
                cout << "Insufficient stock!\n";
                return;
            }

            temp->quantity = temp->quantity + quantity;

            cout << "Cart quantity updated!\n";
            return;
        }

        temp = temp->next;
    }

    if (quantity > p->stock)
    {
        cout << "Insufficient stock!\n";
        return;
    }

    Cart* newItem = new Cart;

    newItem->id = p->id;
    newItem->name = p->name;
    newItem->price = p->price;
    newItem->quantity = quantity;

    if (cart.head == NULL)
    {
        cart.head = newItem;
        cart.tail = newItem;
    }
    else
    {
        cart.tail->next = newItem;
        cart.tail = newItem;
    }

    cout << "Product added to cart!\n";
}



// 13. REMOVE PRODUCT FROM CART


void removeFromCart(CartList& cart)
{
    if (cart.head == NULL)
    {
        cout << "\nCart is empty!\n";
        return;
    }

    int id;

    cout << "\nEnter Product ID to remove: ";
    cin >> id;

    Cart* temp = cart.head;
    Cart* previous = NULL;

    while (temp != NULL && temp->id != id)
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        cout << "Product not found in cart!\n";
        return;
    }

    if (temp == cart.head)
    {
        cart.head = cart.head->next;
    }
    else
    {
        previous->next = temp->next;
    }

    if (temp == cart.tail)
    {
        cart.tail = previous;
    }

    delete temp;

    if (cart.head == NULL)
    {
        cart.tail = NULL;
    }

    cout << "Product removed from cart!\n";
}



// 14. DISPLAY CART AND CALCULATE TOTAL


float displayCart(CartList& cart)
{
    if (cart.head == NULL)
    {
        cout << "\nShopping cart is empty!\n";
        return 0;
    }

    Cart* temp = cart.head;
    float total = 0;

    cout << "\nID\tName\t\tPrice\tQuantity\tAmount\n";

    while (temp != NULL)
    {
        float amount = temp->price * temp->quantity;

        cout << temp->id << "\t"
             << temp->name << "\t\t"
             << temp->price << "\t"
             << temp->quantity << "\t\t"
             << amount << endl;

        total = total + amount;

        temp = temp->next;
    }

    cout << "\nTotal Amount: " << total << endl;

    return total;
}



// 15. CLEAR CART


void clearCart(CartList& cart)
{
    Cart* temp = cart.head;

    while (temp != NULL)
    {
        Cart* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }

    cart.head = NULL;
    cart.tail = NULL;
}


// 16. SAVE ORDERS TO FILE


void saveOrders(OrderQueue& orders)
{
    ofstream file("orders.txt");

    Order* temp = orders.front;

    // Save pending orders
    while (temp != NULL)
    {
        file << temp->orderID << " "
             << temp->numberOfProducts << " "
             << temp->total << " P" << endl;

        temp = temp->next;
    }

    temp = orders.completedHead;

    // Save completed orders
    while (temp != NULL)
    {
        file << temp->orderID << " "
             << temp->numberOfProducts << " "
             << temp->total << " C" << endl;

        temp = temp->next;
    }

    file.close();
}



// 17. PLACE ORDER


void placeOrder(ProductList& products, CartList& cart,
                OrderQueue& orders)
{
    if (cart.head == NULL)
    {
        cout << "\nCannot place order. Cart is empty!\n";
        return;
    }

    Cart* c = cart.head;

    // Check stock again before placing order
    while (c != NULL)
    {
        Product* p = findProduct(products, c->id);

        if (p == NULL || c->quantity > p->stock)
        {
            cout << "Order cannot be placed due to unavailable stock!\n";
            return;
        }

        c = c->next;
    }

    float total = displayCart(cart);

    char confirm;

    cout << "\nConfirm order? (Y/N): ";
    cin >> confirm;

    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "Order cancelled!\n";
        return;
    }

    Order* newOrder = new Order;

    newOrder->orderID = orders.nextOrderID++;
    newOrder->numberOfProducts = 0;
    newOrder->total = total;

    c = cart.head;

    while (c != NULL)
    {
        Product* p = findProduct(products, c->id);

        p->stock = p->stock - c->quantity;

        newOrder->numberOfProducts++;

        c = c->next;
    }

    // Insert order at the rear of queue
    if (orders.front == NULL)
    {
        orders.front = newOrder;
        orders.rear = newOrder;
    }
    else
    {
        orders.rear->next = newOrder;
        orders.rear = newOrder;
    }

    clearCart(cart);

    saveProducts(products);
    saveInventory(products);
    saveOrders(orders);

    cout << "\nOrder placed successfully!\n";
    cout << "Your Order ID is: " << newOrder->orderID << endl;
}



// 18. DISPLAY PENDING ORDERS


void displayOrders(OrderQueue& orders)
{
    if (orders.front == NULL)
    {
        cout << "\nNo pending orders!\n";
        return;
    }

    Order* temp = orders.front;

    cout << "\nPending Orders:\n";

    while (temp != NULL)
    {
        cout << "Order ID: " << temp->orderID
             << " | Products: " << temp->numberOfProducts
             << " | Total: " << temp->total << endl;

        temp = temp->next;
    }
}



// 19. PROCESS NEXT ORDER


void processOrder(OrderQueue& orders)
{
    if (orders.front == NULL)
    {
        cout << "\nNo orders available to process!\n";
        return;
    }

    Order* temp = orders.front;

    cout << "\nProcessing Order ID: "
         << temp->orderID << endl;

    // Remove order from front of queue
    orders.front = orders.front->next;

    if (orders.front == NULL)
    {
        orders.rear = NULL;
    }

    // Move processed order to completed list
    temp->next = NULL;

    if (orders.completedHead == NULL)
    {
        orders.completedHead = temp;
        orders.completedTail = temp;
    }
    else
    {
        orders.completedTail->next = temp;
        orders.completedTail = temp;
    }

    saveOrders(orders);

    cout << "Order processed successfully!\n";
}



// 20. DISPLAY COMPLETED ORDERS


void displayCompletedOrders(OrderQueue& orders)
{
    if (orders.completedHead == NULL)
    {
        cout << "\nNo completed orders!\n";
        return;
    }

    Order* temp = orders.completedHead;

    cout << "\nCompleted Orders:\n";

    while (temp != NULL)
    {
        cout << "Order ID: " << temp->orderID
             << " | Products: " << temp->numberOfProducts
             << " | Total: " << temp->total << endl;

        temp = temp->next;
    }
}



// 21. LOAD PRODUCTS FROM FILE


void loadProducts(ProductList& products)
{
    ifstream file("products.txt");

    if (!file)
    {
        return;
    }

    int id;
    string name, category;
    float price;
    int stock;

    while (file >> id >> name >> category >> price >> stock)
    {
        Product* newProduct = new Product;

        newProduct->id = id;
        newProduct->name = name;
        newProduct->category = category;
        newProduct->price = price;
        newProduct->stock = stock;

        if (products.head == NULL)
        {
            products.head = newProduct;
            products.tail = newProduct;
        }
        else
        {
            products.tail->next = newProduct;
            products.tail = newProduct;
        }
    }

    file.close();
}



// 22. LOAD INVENTORY FROM FILE


void loadInventory(ProductList& products)
{
    ifstream file("inventory.txt");

    if (!file)
    {
        return;
    }

    int id, stock;

    while (file >> id >> stock)
    {
        Product* temp = findProduct(products, id);

        if (temp != NULL)
        {
            temp->stock = stock;
        }
    }

    file.close();
}



// 23. LOAD ORDERS FROM FILE


void loadOrders(OrderQueue& orders)
{
    ifstream file("orders.txt");

    if (!file)
    {
        return;
    }

    int id, numberOfProducts;
    float total;
    char status;

    while (file >> id >> numberOfProducts >> total >> status)
    {
        Order* newOrder = new Order;

        newOrder->orderID = id;
        newOrder->numberOfProducts = numberOfProducts;
        newOrder->total = total;

        if (id >= orders.nextOrderID)
        {
            orders.nextOrderID = id + 1;
        }

        if (status == 'P')
        {
            if (orders.front == NULL)
            {
                orders.front = newOrder;
                orders.rear = newOrder;
            }
            else
            {
                orders.rear->next = newOrder;
                orders.rear = newOrder;
            }
        }
        else if (status == 'C')
        {
            if (orders.completedHead == NULL)
            {
                orders.completedHead = newOrder;
                orders.completedTail = newOrder;
            }
            else
            {
                orders.completedTail->next = newOrder;
                orders.completedTail = newOrder;
            }
        }
        else
        {
            delete newOrder;
        }
    }

    file.close();
}


// 24. FREE MEMORY


void freeMemory(ProductList& products, CartList& cart,
                OrderQueue& orders)
{
    Product* p = products.head;

    while (p != NULL)
    {
        Product* nextNode = p->next;
        delete p;
        p = nextNode;
    }

    Cart* c = cart.head;

    while (c != NULL)
    {
        Cart* nextNode = c->next;
        delete c;
        c = nextNode;
    }

    Order* o = orders.front;

    while (o != NULL)
    {
        Order* nextNode = o->next;
        delete o;
        o = nextNode;
    }

    o = orders.completedHead;

    while (o != NULL)
    {
        Order* nextNode = o->next;
        delete o;
        o = nextNode;
    }

    products.head = NULL;
    products.tail = NULL;

    cart.head = NULL;
    cart.tail = NULL;

    orders.front = NULL;
    orders.rear = NULL;

    orders.completedHead = NULL;
    orders.completedTail = NULL;
}




int main()
{
    ProductList products;
    CartList cart;
    OrderQueue orders;

    // Load previously saved data
    loadProducts(products);
    loadInventory(products);
    loadOrders(orders);

    int choice;

    do
    {
        cout << "\n========== ONLINE SHOPPING SYSTEM ==========\n";

        cout << "1. Add Product\n";
        cout << "2. Display Products\n";
        cout << "3. Update Product\n";
        cout << "4. Delete Product\n";
        cout << "5. Search Product\n";
        cout << "6. Sort Products\n";
        cout << "7. Add Product to Cart\n";
        cout << "8. Remove Product from Cart\n";
        cout << "9. Display Shopping Cart\n";
        cout << "10. Place Order\n";
        cout << "11. Display Pending Orders\n";
        cout << "12. Process Next Order\n";
        cout << "13. Display Completed Orders\n";
        cout << "14. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                addProduct(products);
                break;

            case 2:
                displayProducts(products);
                break;

            case 3:
                updateProduct(products);
                break;

            case 4:
                deleteProduct(products, cart);
                break;

            case 5:
                searchProducts(products);
                break;

            case 6:
                sortProducts(products);
                break;

            case 7:
                addToCart(products, cart);
                break;

            case 8:
                removeFromCart(cart);
                break;

            case 9:
                displayCart(cart);
                break;

            case 10:
                placeOrder(products, cart, orders);
                break;

            case 11:
                displayOrders(orders);
                break;

            case 12:
                processOrder(orders);
                break;

            case 13:
                displayCompletedOrders(orders);
                break;

            case 14:
                saveProducts(products);
                saveInventory(products);
                saveOrders(orders);

                freeMemory(products, cart, orders);

                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "\nInvalid choice! Try again.\n";
        }

    } while (choice != 14);

    return 0;
}
