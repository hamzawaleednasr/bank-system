# 🏦 Bank System – C++ Console Application

A **fully-featured banking system** built with **modern C++**, designed to simulate real-world banking operations with a strong focus on **clean architecture**, **security**, and **maintainability**.

This project demonstrates solid software engineering practices, including role-based access control, input validation, file-based persistence, and controller-driven application flow.

---

## 📌 Overview

The **Bank System** is a console-based application that allows authorized users to manage clients, perform financial transactions, and administer system users.

All operations are protected by a **permission system**, ensuring that each user can only access what they are allowed to.

The project is built without external frameworks, relying purely on **C++ and STL**, making it a strong example of clean procedural design with clear separation of concerns.

---

## ✨ Features

### 🔐 Authentication & Authorization
- Secure login system
- Passwords stored as **hashed values** (no plain-text passwords)
- Role-based access control using **bitwise permissions**
- Full-access administrator support

---

### 👥 Client Management
- Add new clients with unique account numbers
- Update client information
- Delete clients safely
- Search and display client details
- Persistent storage using text files

---

### 💰 Transactions
- Deposit money into client accounts
- Withdraw money with balance validation
- Automatic balance updates
- Transaction-safe file persistence

---

### 👤 User Management
- Add, update, delete, and search system users
- Assign granular permissions per user
- Prevent deletion of full-access administrators
- Masked password display

---

### 🧾 Logging & Time Handling
- Centralized date-time generation using `std::chrono`
- Timestamp-ready logging system
- Cross-platform time formatting (Windows / Linux)

---

## 🧠 Technical Highlights

- Written in **Modern C++ (C++11+)**
- Clear separation of responsibilities:
  - Controllers
  - UI / Display layer
  - File handling
  - Validation
  - Core business logic
- Robust input validation
- Bitwise permission system
- File-based persistence (no database dependency)
- Readable, maintainable, and scalable codebase

---

## 🔑 Permissions System

Permissions are implemented using **bitwise flags**, allowing flexible and efficient access control:

| Permission        | Description                  |
|-------------------|------------------------------|
| LIST              | View clients                 |
| ADD               | Add new clients              |
| DELETE            | Delete clients               |
| UPDATE            | Update client data           |
| FIND              | Search clients               |
| TRANS             | Perform transactions         |
| MANAGE_USERS      | Manage system users          |
| FULL ACCESS       | All permissions              |

---

## 📁 Logical Project Structure

```text
BankSystem/
│
├── Authentication (Login / Logout)
├── Client Management
├── User Management
├── Transactions
├── Permissions System
├── File Handling Layer
├── Validation Utilities
├── Logging & Time Utilities
└── Controllers Layer
```

---

## 🔐 Security Notes

 - Passwords are never stored in plain text
 - Hashing is applied before persistence
 - Permission system uses bitwise flags
 - Access checks are enforced before every sensitive operation

 --- 

## 👨‍💻 Author

**Hamza Waleed**

**Full-Stack Developer**
