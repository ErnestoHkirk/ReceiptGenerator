/*
    Name: Ernesto, Dylan
    CSCI 123
    Date: Dec 05, 2018

*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iomanip>
using namespace std;

class Product
{
	private:
		int barcode;
		string name;
		double price;
		string category;
		int quantity;
		// double salePrice;
		double totalPrice;
  public:
		Product(int pBarcode, string pName, string pCategory, double pPrice, int pQuantity);
		string getName();
		string getCategory();
		double getPrice();
		double getTotalPrice();
		int getBarcode();
		int getQuantity();
		void increaseQuantity();
		void setTotalPrice();
};

void productObjectCreator(string purchaseFileName, string productsFileName, vector<Product*> &products);
//Pre: takes in two file names as strings and a vector of pointers pointing to Product objects
//Post: creates a vector of product objects based on the products the barcodes in the purchasefile with
//all the information on each product

void sortByCategory(vector<Product*> &productObj);
//Pre: takes a vector of pointers pointing to Product objects
//Post: sorts the objects in the vector by category and puts them back into the vector sorted

bool barcodeIsElement(int numBarcode, vector<int> barcodes);
//Pre: takes a barcode and an array of barcodes to check against
//Post: checks if the barcode provided is an element in the vector of barcodes, then returns
//true or false

double subTotalPrice(vector<Product*> products);
//Pre: takes a vector of pointers pointing to Product objects
//Post: returns the total price of all the product objects in the provided vector

void setTotalPricesOfProducts(vector<Product*> products);
//Pre: takes a vector of pointers pointing to Product objects
//Post: sets all total price for all products based on quantity * price

void printHeader();
//prints header section of the receipt

void printReceiptTimeCard();
//used for card purchases and prints out payment information and valadation from credit cards

void printReceiptTimeCash();
//used for cash purchaes and prints out payment information

void printReceiptProducts(vector<Product*> products);
//Pre: takes a vector of pointers pointing to Product objects
//Post: prints category and product bacrode, name and price of all the products in that category, and if quantity
//is greater than 1, it prints quantity and price for single item. then moves to next category until all products
//are printed

void printReceiptFinish();
//prints footer section of receipt

char* asctime1(const struct tm *timeptr);
//Pre: takes time structure proivded by library
//Post: returns date and time information

void printReceiptTotals(double subTotal, double taxRate, int amountItemsSold, double amountReceived);
//Pre: takes subTotal, tax rate ind decimal, amount of items sold and amount recieved from user to pay for productsDatabase
//Post: prints out subtotal, tax and total on receipt along with amount of items sold and change due to user

int amountItemsSold(vector<Product*> products);
//Pre: takes a vector of pointers pointing to Product objects
//Post: counts total quantity of products, not just number of elements in vector but the quantity of each product



int main()
{
	string purchaseFileName = "purchase.txt"; //purchase file names
	string productsFileName = "products.txt"; //product database file with all products available

	double taxRate = 0.08; //8%

	vector<Product*> products; //main vector to hold objects of products being purchased

	productObjectCreator(purchaseFileName, productsFileName, products);\
	sortByCategory(products);
	setTotalPricesOfProducts(products);

	double subTotal = subTotalPrice(products);
	double finalAmountDue = (subTotal * taxRate) + subTotal;
	string paymentType;
	double paymentReceived;


	//Prompts user on how they will pay
	cout << "Thank you for shopping at Walmart. Your total for today is $" << fixed << setprecision(2) << finalAmountDue << endl;
	cout << "How will you be paying today? Cash or Card: ";
	cin >> paymentType;

	//if cash, user is prompted for amount of money to use for transaction and also uses printReceiptTimeCash on receipt
	//if card, it uses printReceiptTimeCard on receipt
	if(paymentType == "cash" || paymentType == "Cash")
	{
		cout << "Thank you. Please provide the amount of cash you are providing: ";
		cin >> paymentReceived;
		cout << "Thank you and here is your receipt. Please have a wonderful day." << endl;
		printHeader();
		printReceiptProducts(products);
		printReceiptTotals(subTotalPrice(products), taxRate, amountItemsSold(products), paymentReceived);
		printReceiptTimeCash();
		printReceiptFinish();
	}
	else if(paymentType == "card" || paymentType == "Card")
	{
		cout << "Thank you and here is your receipt. Please have a wonderful day." << endl;
		paymentReceived = finalAmountDue;
		printHeader();
		printReceiptProducts(products);
		printReceiptTotals(subTotalPrice(products), taxRate, amountItemsSold(products), paymentReceived);
		printReceiptTimeCard();
		printReceiptFinish();
	} else
	{
		cout << "Sorry that was an invaild entry, please try restart." << endl;
	}


  return 0;
}//END main()

//constructor for Product class
Product::Product(int pBarcode, string pName, string pCategory, double pPrice, int pQuantity){
	barcode = pBarcode;
  name = pName;
  price = pPrice;
  category = pCategory;
  quantity = pQuantity;
	totalPrice = pPrice * 1;
}
//getter fn for product class
int Product::getBarcode(){
	return barcode;
}
//getter fn for product class
string Product::getName(){
  return name;
}
//getter fn for product class
string Product::getCategory(){
  return category;
}
//getter fn for product class
double Product::getPrice(){
  return price;
}
//getter fn for product class
double Product::getTotalPrice(){
	return totalPrice;
}
//getter fn for product class
int Product::getQuantity(){
  return quantity;
}
//setter fn for product class quantity, increaes quantity by one
void Product::increaseQuantity(){
	quantity++;
}
//setter fn for product class for total price of individual product based
//on amount of that product being purchased
void Product::setTotalPrice(){
	totalPrice = quantity * price;
}

//Pre: takes in two file names as strings and a vector of pointers pointing to Product objects
//Post: creates a vector of product objects based on the products the barcodes in the purchasefile with
//all the information on each product
void productObjectCreator(string purchaseFileName, string productsFileName, vector<Product*> &products)
{
	//create and open purchaseInput file and stream
	ifstream purchaseInput;
	purchaseInput.open("purchase.txt");

	string purchaseBarCode;
	string productBarCode;

	int productObjectCount = 0;

	string putLine; //var for line to be put back into file
	string oldProductLine; //var for product line that hasn't been updated to find in input file and then to be replaced by new line
	string productsDatabaseLine; //var to hold line taken from input file to be used as stringstream

	string clearData, pName, pCategory, pPrice, pQuantity;
	//vars to store product information to be used for product object creation

	vector<int> barcodeExsistsArray;
	//vector to hold barcodes of products that have objects already created for them

	//reads through each barcode in the purchase file
	while(purchaseInput >> purchaseBarCode)
	{
		//create and open product data base file as well as temp storage file
		ifstream productsDatabase;
		productsDatabase.open("products.txt");
		ofstream tempFile;
		tempFile.open("temp.txt");


		//loops through products database file and stores each line one at a time so it change be
		//checked against items in purchase file
		while(getline(productsDatabase,productsDatabaseLine))
		{

			//var to hold each line as a stream so that data can be extracted
			stringstream lineStream(productsDatabaseLine);
			//stores current element's barcode of product file
			getline(lineStream, productBarCode, ',');

			//compare current barcode of purchase file and current barcode of product database file
			if(purchaseBarCode == productBarCode)
			{

				//changes barcode from string to int (stoi was causing errors)
				int numBarcode;
				stringstream barcodeOne(productBarCode);
				barcodeOne >> numBarcode;

				 //if barcode exists, increae quantity and set total prices
				 //puts data into vars so that we can adjust quanitity and put back into product database file
				if(barcodeIsElement(numBarcode, barcodeExsistsArray))
				{
					// int currentBarcode;
					for(int i = 0; i < products.size(); i++){
						int currentBarcode = products[i]->getBarcode();
						if(currentBarcode == numBarcode)
								products[i]->increaseQuantity();
					}
					//stores each piece of product data in individual vars so it can be accessed
					getline(lineStream, pName, ',');
					getline(lineStream, pCategory, ',');
					getline(lineStream, pPrice, ',');
					getline(lineStream, pQuantity);

					//changes quantity from string to int (stoi was causing errors)
					int numQuantity;
					stringstream quantityOne(pQuantity);
					quantityOne >> numQuantity;

					//changes barcode from string to double (stoi was causing errors)
					double numPrice;
					stringstream priceOne(pPrice);
					priceOne >> numPrice;

					//current product line from product database file
					oldProductLine = productBarCode + "," + pName + "," + pCategory + "," + pPrice + "," + pQuantity;
					//reduces quantity of current product by 1
					numQuantity--;
					//creates new product line with quantity minus 1 to be put back into product database file
					putLine = productBarCode + "," + pName + "," + pCategory + "," + pPrice + "," + to_string(numQuantity);
					//product line reconstructed minus 1 for quantity

					//replaces current product line for updated product line to be put back into product database file
					oldProductLine.replace(oldProductLine.begin(), oldProductLine.end(), putLine);
				}
				else //create product object
				{
					//stores each piece of product data in individual vars so it can be accessed
					getline(lineStream, pName, ',');
					getline(lineStream, pCategory, ',');
					getline(lineStream, pPrice, ',');
					getline(lineStream, pQuantity);

					//changes quantity from string to int (stoi was causing errors)
					int numQuantity;
					stringstream quantityOne(pQuantity);
					quantityOne >> numQuantity;

					//changes barcode from string to double (stoi was causing errors)
					double numPrice;
					stringstream priceOne(pPrice);
					priceOne >> numPrice;

					//creates a new temporary Product object with information from current line of product tempFile, sets the quantity to one
					//and pushes the object into the products vector
					products.push_back(new Product(numBarcode, pName, pCategory, numPrice, 1));

					//puts barcode into array to be checked against
					barcodeExsistsArray.push_back(numBarcode);

					//current product line from product database file
					oldProductLine = productBarCode + "," + pName + "," + pCategory + "," + pPrice + "," + pQuantity;
					//reduces quantity of current product by 1
					numQuantity--;
					//creates new product line with quantity minus 1 to be put back into product database file
					putLine = productBarCode + "," + pName + "," + pCategory + "," + pPrice + "," + to_string(numQuantity);
					//product line reconstructed minus 1 for quantity

					//replaces current product line for updated product line to be put back into product database file
					oldProductLine.replace(oldProductLine.begin(), oldProductLine.end(), putLine);
				}

			}
			else //if no match, put line back into file
			{
				//if current product line doesn't match purchase barcode then put the line back together with no change to be
				//put back into product database file
				getline(lineStream, putLine);
				putLine = productBarCode + "," + putLine;
			}

			//puts product line into temp file so it can be updated
			tempFile << putLine << endl;

		}


		//saves temp file as new product database file with updated quantities
		tempFile.close();
		remove("products.txt");
		rename("temp.txt", "products.txt");
		productsDatabase.close();

		//resets productFile in order to loop through it again
		productsDatabase.clear();
		productsDatabase.seekg(0, std::ios::beg);


	}//END purchaseFile while loop

	purchaseInput.close();
}//END productObjectCreator();


//Pre: takes a vector of pointers pointing to Product objects
//Post: sorts the objects in the vector by category and puts them back into the vector sorted
void sortByCategory(vector<Product*> &productObj)
{
	string categoryArray[] = {"Frozen", "Meat", "Dairy", "Produce", "Snacks"};
	//array holding all possible category types for products
  string temp;
  vector<Product*> tempProducts;

	//loops through categories array to find matching products, then pushes them into temp vector
	//goes through each category
	for (int i = 0; i < 5; i++)
	{
		for(int j = 0; j < productObj.size(); j++)
    {
    temp = productObj[j]->getCategory();
    if (categoryArray[i] == temp)
    	{
      tempProducts.push_back(productObj[j]);
    	}
    }
  }
	//pushes products sorted by category back into products vector
  for (int i = 0; i < tempProducts.size(); i++)
  	{
  	productObj[i] = tempProducts[i];
		}
  return;
}

//Pre: takes a barcode and an array of barcodes to check against
//Post: checks if the barcode provided is an element in the vector of barcodes, then returns
//true or false
bool barcodeIsElement(int numBarcode, vector<int> barcodes)
{
	for(int i = 0; i < barcodes.size(); i++)
	{
		if(numBarcode == barcodes[i]){
			return true;
		}
	}
	return false;
}

//prints header section of the receipt
void printHeader()
{
	cout << "_______________________________________________________" << endl;
	cout << "|/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\|" << endl;
	cout << "|\\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/|" << endl;
	cout << "|-----------------------------------------------------|" << endl;
	cout << "|                                                     |" << endl;
	cout << "| \\      /\\      /   |                 |     \\  /     |" << endl;
	cout << "|  \\    /  \\    / __|| _    _  __||__ _|_   __\\/__    |" << endl;
	cout << "|   \\  /    \\  / |  ||| \\  / ||  ||    |      /\\      |" << endl;
	cout << "|    \\/      \\/  |__|||  \\/  ||__||    |     /  \\     |" << endl;
	cout << "|                                                     |" << endl;
	cout << "|            Save money. Live better.                 |" << endl;
	cout << "|-----------------------------------------------------|" << endl;
	cout << "|/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\|" << endl;
	cout << "|\\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/|" << endl;
	cout << "|-----------------------------------------------------|" << endl;
	cout << "|              (123) 456 - 7890                       |" << endl;
	cout << "|              MANAGER JOHN DOE                       |" << endl;
	cout << "|           111 SAMPLE CROSSING BLVD                  |" << endl;
	cout << "|             MARYSVILLE OH 43000                     |" << endl;
	cout << "|               www.walmart.com                       |" << endl;
	cout << "|     ST# 1738  OP# 00001234 TE# 12  TR#01234         |" << endl;
	cout << "|-----------------------------------------------------|" << endl;
}

//used for cash purchaes and prints out payment information
void printReceiptTimeCash()
{
    // Declaring argument for time()
    time_t tt;

    // Declaring variable to store return value of
    // localtime()
    struct tm * ti;

    // Applying time()
    time (&tt);

    // Using localtime()
    ti = localtime(&tt);

    cout << "| Payment Type:            Cash                       |" << endl;
    cout << "| ACCOUNT #           **** **** ****                  |" << endl;
    cout << "| APPROVAL # 01234A                                   |" << endl;
    cout << "| REF #01234568901                                    |" << endl;
    cout << "| TRANS ID - 012345678901234                          |" << endl;
    cout << "| VALIDATION  - 3QBR                                  |" << endl;
    cout << "| PAYMENT SERVICE - E                                 |" << endl;
    cout << "| TERMINAL # 3000202020                               |" << endl;
    cout << "|                                                     |" << endl;
    cout << "|               " << asctime1(ti)   << "              |" << endl;
}

//used for card purchases and prints out payment information and valadation from credit cards
void printReceiptTimeCard()
{
    // Declaring argument for time()
    time_t tt;

    // Declaring variable to store return value of
    // localtime()
    struct tm * ti;

    // Applying time()
    time (&tt);

    // Using localtime()

    ti = localtime(&tt);
    cout << "|                      --VISA--                       |" << endl;
    cout << "|                    **APPROVED**                     |" << endl;
    cout << "| ACCOUNT #        **** **** **** 4324                |" << endl;
    cout << "| APPROVAL # 01234A                                   |" << endl;
    cout << "| REF #01234568901                                    |" << endl;
    cout << "| TRANS ID - 012345678901234                          |" << endl;
    cout << "| VALIDATION  - 3QBR                                  |" << endl;
    cout << "| PAYMENT SERVICE - E                                 |" << endl;
    cout << "| TERMINAL # 3000202020                               |" << endl;
    cout << "|                                                     |" << endl;
    cout << "|               " << asctime1(ti)   << "              |" << endl;
}

//prints footer section of receipt
void printReceiptFinish()
{
  // Declaring argument for time()
    time_t tt;

    // Declaring variable to store return value of
    // localtime()
    struct tm * ti;

    // Applying time()
    time (&tt);

    // Using localtime()
    ti = localtime(&tt);

    cout << "|                                                     |" << endl;
    cout << "|    TC #7123 0000 0000 1234                          |" << endl;
    cout << "|                                                     |" << endl;
    cout << "|   || ||| ||  ||  | |  |  |  |||| | |  || |  ||  |   |" << endl;
    cout << "|   || ||| ||  ||  | |  |  |  |||| | |  || |  ||  |   |" << endl;
    cout << "|   || ||| ||  ||  | |  |  |  |||| | |  || |  ||  |   |" << endl;
    cout << "|                                                     |" << endl;
    cout << "|        Low prices. Every day. On everything         |" << endl;
    cout << "|         Backed by our Ad Match Guarantee.           |" << endl;
    cout << "|               " << asctime1(ti)   << "              |" << endl;
    cout << "|                                                     |" << endl;
    cout << "|               ***CUSTOMER COPY***                   |" << endl;
    cout << "|_____________________________________________________|" << endl;
}

//Pre: takes a vector of pointers pointing to Product objects
//Post: prints category and product bacrode, name and price of all the products in that category, and if quantity
//is greater than 1, it prints quantity and price for single item. then moves to next category until all products are printed
void printReceiptProducts(vector<Product*> products)
{
 string currentCat; //var to hold current category of products being printed

 //vars for formating adjustments
 int maxStringLength = 50;
 int lineAdjustLength = 0,
 		 barcodeLineAdjustLength = 0,
		 nameLineAdjustLength = 0,
		 priceLineAdjustLength = 0,
		 quantityLineAdjustLength = 0,
		 indyPriceLineAdjustLength = 0;


 for(int i = 0; i < products.size(); i++){

	 //sets category and prints the category to recepit, once a product comes up with a new category
	 //that category is then printed and so on
	 string currentProductCat = products[i]->getCategory();
	 if(currentCat != currentProductCat){
		 currentCat = currentProductCat;
		 lineAdjustLength = maxStringLength - currentCat.length();
		 for(auto& x: currentProductCat)
				x = toupper(x);
		 cout << "|";
	   cout.width(54);
	   cout << "|" << endl;
		 cout << "|    " << currentProductCat << setw(lineAdjustLength) << "|" << endl;
	 }

	 //set up adjustments for formating of products being printed to receipt
	 string barcodeString = to_string(products[i]->getBarcode());
	 barcodeLineAdjustLength = 8 - barcodeString.length();
	 string nameString = products[i]->getName();
	 nameLineAdjustLength = 23 - nameString.length() + barcodeLineAdjustLength;
	 string priceString = to_string(products[i]->getTotalPrice());
	 priceLineAdjustLength = 5;

	 //actual code that prints the products to receipt
	 cout << "|    " << barcodeString << setw(barcodeLineAdjustLength) << "      " << nameString << setw(nameLineAdjustLength) << "   ";
	 cout.width(6 + barcodeString.length());
	 cout	<< fixed << setprecision(2) << right << products[i]->getTotalPrice() << setw(priceLineAdjustLength) << "   |" << endl;

	 //set up adjustments for formating of quantity lines
	 quantityLineAdjustLength = 3 - to_string(products[i]->getQuantity()).length();
	 indyPriceLineAdjustLength = 42 - to_string(products[i]->getPrice()).length();

	 //when product has more than one quantity, the 'amount @ indivdual price' is printed to receipt
	 if((products[i]->getQuantity()) > 1){
		 cout << "|            " << products[i]->getQuantity() << setw(quantityLineAdjustLength);
		 cout << " @ ";
		 cout << products[i]->getPrice() << setw(indyPriceLineAdjustLength) << "|" << endl;
	 }
 }//END for loop

 //formating blank lines in receipt
 cout << "|";
 cout.width(54);
 cout << "|" << endl;
 cout << "|";
 cout.width(54);
 cout << "|" << endl;
}

//Pre: takes subTotal, tax rate ind decimal, amount of items sold and amount recieved from user to pay for productsDatabase
//Post: prints out subtotal, tax and total on receipt along with amount of items sold and change due to user
void printReceiptTotals(double subTotal, double taxRate, int amountItemsSold, double amountReceived)
{
	//calculates taxAmount, final price and change due
	double taxAmount = subTotal * taxRate;
	double absoluteTotalPrice = taxAmount + subTotal;
	double changeDue = amountReceived - absoluteTotalPrice;

	//set up adjustments for formating of totals being printed to receipt
	int subTotalLength = to_string(subTotal).length();
	int subTotalLineAdjust = 10 - subTotalLength;
	int taxLength = to_string(taxAmount).length();
	int taxLineAdjust = 10 - taxLength;
	int absoluteTotalLength = to_string(absoluteTotalPrice).length();
	int absoluteTotalLineAdjust = 10 - absoluteTotalLength;

	//actual code that prints subtotal, tax amount, and total price
	cout << "|";
	cout.width(38);
	cout << right << "SUBTOTAL: ";
	cout.width(15 - subTotalLength);
  cout << "$" << fixed << setprecision(2) << subTotal << setw(subTotalLineAdjust) << "    |" << endl;
	cout << "|";
	cout.width(31);
	cout << right << "TAX " << (taxRate * 100) << "%: ";
	cout.width(15 - taxLength);
	cout << "$" << fixed << setprecision(2) << taxAmount << setw(taxLineAdjust) << "    |" << endl;
	cout << "|";
	cout.width(38);
	cout << right << "TOTAL: ";
	cout.width(15 - absoluteTotalLength);
	cout << "$" << fixed << setprecision(2) << absoluteTotalPrice << setw(absoluteTotalLineAdjust) << "    |" << endl;

	//formating of blank lines in receipt
	cout << "|";
	cout.width(54);
	cout << "|" << endl;
	cout << "|";
	cout.width(54);
	cout << "|" << endl;

	//set up for amount of items sold and code that prints out amount of items sold
	int amountItemsSoldLength = to_string(amountItemsSold).length();
	cout << "|";
	cout.width(38);
	cout << right << "#ITEMS SOLD: ";
	if(amountItemsSold > 9)
		cout.width(13 - amountItemsSoldLength);
	else
		cout.width(12 - amountItemsSoldLength);
	cout << right << amountItemsSold << setw(5) << "    |" << endl;

	//set up for formation of change due and code that prints out change due
	int changeDueLength = to_string(changeDue).length();
	cout << "|";
	cout.width(38);
	cout << right << "CHANGE DUE: ";
	cout.width(15 - changeDueLength);
	cout << right << "$" << fixed << setprecision(2) << changeDue << setw(4) << "    |" << endl;

	//formating of blank lines in receipt
	cout << "|";
	cout.width(54);
	cout << "|" << endl;
	cout << "|";
	cout.width(54);
	cout << "|" << endl;

}

//Pre: takes time structure proivded by library
//Post: returns date and time information
char* asctime1(const struct tm *timeptr)
{
  static const char wday_name[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  };
  static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[26];
  sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
    wday_name[timeptr->tm_wday],
    mon_name[timeptr->tm_mon],
    timeptr->tm_mday, timeptr->tm_hour,
    timeptr->tm_min, timeptr->tm_sec,
    1900 + timeptr->tm_year);
  return result;
}

//Pre: takes a vector of pointers pointing to Product objects
//Post: returns the total price of all the product objects in the provided vector
double subTotalPrice(vector<Product*> products)
{
	double subTotal = 0.0;
	for(int i = 0; i < products.size(); i++){
		subTotal += products[i]->getTotalPrice();
	}
	return subTotal;
}

//Pre: takes a vector of pointers pointing to Product objects
//Post: sets all total price for all products based on quantity * price
void setTotalPricesOfProducts(vector<Product*> products)
{
	for(int i = 0; i < products.size(); i++){
		products[i]->setTotalPrice();
	}
}

//Pre: takes a vector of pointers pointing to Product objects
//Post: counts total quantity of products, not just number of elements in vector but the quantity of each product
int amountItemsSold(vector<Product*> products)
{
	int itemAmount = 0;
	for(int i = 0; i < products.size(); i++){
		itemAmount += products[i]->getQuantity();
	}
	return itemAmount;
}
