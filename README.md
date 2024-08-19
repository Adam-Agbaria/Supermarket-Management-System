# SuperMarket Management System

This project implements a SuperMarket Management System using the C programming language, adhering to Object-Oriented Programming (OOP) principles. The system facilitates customer registration, product inventory management, shopping processes, and file storage. It includes dynamic memory management, linked lists, and supports data compression for efficient storage.

## Features

- **Customer Management**: Includes customer registration, purchase tracking, and dynamic shopping carts.
- **Product Management**: Supports various product types, each with a unique barcode, and allows for dynamic inventory updates.
- **SuperMarket Management**: Manages customers and products using dynamic arrays and linked lists.
- **Shopping Process**: Enables customers to add products to their cart, calculate totals, and proceed to checkout.
- **File Operations**: Supports saving and loading data from both binary and text files, with optional compression.
- **Sorting and Searching**: Customers can be sorted by various criteria, with binary search functionality.
- **Memory Management**: Efficient handling of dynamic memory to prevent memory leaks.


### Requirements
- **Operating System**: Windows OS
- **Development Environment**: Visual Studio with C/C++ environment

# Build and Run Instructions

## Build

1. Open the project in Visual Studio.
2. Build the solution by navigating to `Build > Build Solution`.

## Run

- Run the project by pressing `Ctrl+F5` or go to `Debug > Start Without Debugging`.

## Usage

After launching the program, you will have access to the following options:

1. **Print SuperMarket Details**
2. **Add Product**
3. **Add Customer**
4. **Start Customer Purchase**
5. **Print Shopping Cart**
6. **Checkout**
7. **Print Products by Type**
8. **Exit**

### Advanced Options

- **Sorting**: Sort customers by name, number of purchases, or total expenditure.
- **Searching**: Perform binary searches on sorted customers.
- **File Operations**: Data is saved to and loaded from `SuperMarket.bin` (binary file) and `Customers.txt` (text file).

## File Structure

- **Binary File (`SuperMarket.bin`)**: Stores SuperMarket details and products, with optional compression.
- **Text File (`Customers.txt`)**: Stores customer details, including purchase history.

## Memory Management

- **Dynamic Arrays**: Arrays for customers and products are automatically resized as needed.
- **Linked Lists**: Products are stored in a linked list, sorted by barcode.
- **Memory Cleanup**: All dynamically allocated memory is properly freed when the program exits.

## Error Handling

Custom macros are implemented for error handling and resource management to ensure consistent and reliable code practices throughout the project.

## Contributing

1. Fork the repository.
2. Create a new branch.
3. Submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

