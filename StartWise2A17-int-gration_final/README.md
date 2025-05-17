# StartWise2A17-int-gration_final

## Project Overview
This project is a Qt-based application designed to manage employee access through a login system. Upon launching the application, users are presented with a login page. Successful authentication grants access to the main application interface.

## File Structure
- **src/**
  - **main.cpp**: Entry point of the application that initializes and displays the login page.
  - **mainwindow.h**: Header file for the `MainWindow` class, representing the main interface post-login.
  - **mainwindow.cpp**: Implementation of the `MainWindow` class, managing the main application window.
  - **login.h**: Header file for the `Login` class, handling user input and credential validation.
  - **login.cpp**: Implementation of the `Login` class, including UI display and credential checking logic.
  - **login.ui**: User interface definition for the login page, created with Qt Designer.
  - **arduin.h**: Header file for the `arduin` class, managing Arduino device connections.
  - **arduin.cpp**: Implementation of the `arduin` class for communication with the Arduino.
  - **database/**
    - **database_connection.h**: Header file for the database connection class, managing database interactions for employee credentials.
  
- **resources/**
  - **styles.qss**: Stylesheet for custom UI styling.

- **CMakeLists.txt**: Build configuration file for CMake.

- **README.md**: Documentation for project setup and usage.

## Setup Instructions
1. **Clone the Repository**: 
   ```
   git clone <repository-url>
   ```
2. **Navigate to the Project Directory**:
   ```
   cd StartWise2A17-int-gration_final
   ```
3. **Build the Project**:
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```
4. **Run the Application**:
   ```
   ./YourApplicationExecutable
   ```

## Usage
- Upon launching, the login page will appear. Enter your credentials to access the main application.
- The application validates credentials against a database of employees.
- If login is successful, the main window will be displayed for further interactions.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue for any enhancements or bug fixes.

## License
This project is licensed under the MIT License. See the LICENSE file for details.