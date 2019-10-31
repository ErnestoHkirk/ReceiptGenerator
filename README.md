# ReceiptPrinter
Program to output formatted receipt to purchaser updating current inventory 

#### CSCI 123–Final Project: Receipt Printer ####
##### Ernesto, Dylan #####

### Purpose of the Project ###
The purpose of this project is to take in a file with items to be purchased from user, then
calculate the subtotal, tax and final price of the products. The user is also prompted for cash or card,
and if the user supplies cash, the program calculates the change that will be due. After this is done, a
recipet is printed out for the user.

### Program Analysis and Program Design ###
Our program resolves around creating objects of our Product class and storing them within a
vector that is sorted and used throughout the program. Within that main idea, we have three main
parts: the product object creator function, the sorting function and the printing functions.
The Product class was made up of variables that would be needed about each product to
print to the receipt, including: barcode, name, price, category, quantity and total price. The product
class had all the needed getter and setter functions along with one constructor that is used in the
productObjectCreator function. The variables used inside the main function held the file names for
the input and output files along with constants for the program including, the tax rate and
paymentType. The most important variable is the products vector which stores all the Product
objects of products being purchased. Along with that are varous variables used to calculate the
prices and change due to user.

The product object creator function is the most important part of the program. It begins by
looping through the purchase file, which holds the barcodes of items to be purchased, and the
products database file, which holds all the products available to be purchased, so that matches can
be found. Once a match is found, the corresponding product line is stored in a stringstream variable
to allow extraction of data about the particular product. Once the data about the product is extracted
into individual variables, the product data is used to create a dynamic Product object which is
pushed into the main products vector. The product quantity is then reduced by one and all the data
of that product is put back into a string and replaces the line that was extracted from the products
database file. The barcode for that product is also stored into a vector so it can be checked with the
barcodeIsElement function to see if the product already has an obejct created for it. When a
duplicate product is added, the function increases the quantity of the corresponding product in the
main product vector, then updates the quantity in the product database file, just like when a new
product is inputted, except a new object is not created.

The sorting function does as it states by sorting the Product objects in the main product
vector by category. This is accomplished by looping through an array of the category names and the
main product vector. The first category products are pushed into a temporary vector first, followed
by the second and so on, until all products are sorted. After, the temporary vector elements
overwrite the main products vector so that when it is accessed later, all the product objects are
sorted and ready to be printed. The last section is the printing functions, which are pretty self
explanatory. The subtotal, tax and final prices are calculated by looping through the main product
vector. Then we used the iomanip library along with other built in functions to format and print the
receipt.

# PROGRAM OUTPUT / SAMPLE RUNS #
## Receipt if Payment is in Cash: ##

![image](https://user-images.githubusercontent.com/37064367/67988306-d3749f80-fbec-11e9-95df-c61e888a4fdb.png)

![image](https://user-images.githubusercontent.com/37064367/67988315-d66f9000-fbec-11e9-9e72-d1db23583dcc.png)

# PROGRAM OUTPUT / SAMPLE RUNS #
## Receipt if Payment is by Card: ##

![image](https://user-images.githubusercontent.com/37064367/67988321-d96a8080-fbec-11e9-92fc-658634388c5e.png)

![image](https://user-images.githubusercontent.com/37064367/67988326-db344400-fbec-11e9-9124-7510b9a79411.png)
