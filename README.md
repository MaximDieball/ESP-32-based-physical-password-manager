
# ESP32-Based Physical Password Manager Device

This project is a **physical password manager** built on an **ESP32** (LilyGO T-Deck).  
It securely stores all your usernames and passwords in an **encrypted** format on the device with **no internet connection**, allowing you to quickly look them up.  
The device can also be plugged into a computer and act as a USB HID keyboard to **automatically type out complex passwords** with one click. No more entering them character by character.

## Motivation / Problem

I created this project because I was frustrated with manually copying passwords from my phone or using weaker easy to remember passwords when working on non-personal computers.  
When logging into a site on a library or school computer, I didn’t want to log into my password manager and risk exposing all my credentials.

To solve this, I built a **portable physical password manager** that can be used anywhere without giving the host computer access to your entire password vault.  
While a smartphone password manager could do something similar, this device goes a step further by **automatically typing in usernames and passwords**, making the process faster and letting you use longer, more secure passwords. Because the passwords are saved on the device it is also safer than trusting a third party cloud provider with your passwords.


# ESP32-Based Physical Password Manager Device

This project is a **physical password manager** built on an **ESP32** (LilyGO T-Deck).  
It securely stores all your usernames and passwords in an **encrypted** format on the device with **no internet connection**, allowing you to quickly look them up.  
The device can also be plugged into a computer and act as a USB HID keyboard to **automatically type out complex passwords** with one click. No more entering them character by character.

## Motivation / Problem

I created this project because I was frustrated with manually copying passwords from my phone or using weaker easy to remember passwords when working on non-personal computers.  
When logging into a site on a library or school computer, I didn’t want to log into my password manager and risk exposing all my credentials.

To solve this, I built a **portable physical password manager** that can be used anywhere without giving the host computer access to your entire password vault.  
While a smartphone password manager could do something similar, this device goes a step further by **automatically typing in usernames and passwords**, making the process faster and letting you use longer, more secure passwords. Because the passwords are saved on the device it is also safer than trusting a third party cloud provider with your passwords.


## Features

-   **Store**  website, username, and password.
    
-   **Encrypt**  credentials using  **AES**.
    
-   **Generate secure passwords**.
    
-   **Add, delete, and edit**  credentials.
    
-   **Type**  username, password, or both, with automatic tab between input fields.
    

#### Coming Soon

-   **Searchable credentials**
    
-   **2FA tokens**
    
-   **Themes**
    

----------

## How to Use the Device

### Set Master Password

1.  Connect the device for the first time.
    
2.  Enter your master password.
    

### Add a Password

1.  **Unlock**  the device.
    
2.  Go to  **Passwords**.
    
3.  Press the  **"+" button**  in the top right.
    
4.  Enter  **website**,  **username**, and  **password**.
    
5.  **Confirm**  to save.
    

### Look Up and Type Credentials

1.  **Unlock**  the device.
    
2.  Go to  **Passwords**.
    
3.  Select your  **password/website**.
    
4.  Press  **"Show Username"**  or  **"Show Password"**  to display them on the screen.
    
5.  Press the  **"Type"**  button next to a username or password to type it individually.
    
6.  Press  **"Type Both"**  to type both with a tab in between.
    

### Generate a Secure Password

-   Press the  **"Gen" button**  on the password entry screen when adding or editing a password.
    

### Edit a Password

1.  **Unlock**  the device.
    
2.  Go to  **Passwords**.
    
3.  Select the  **password/website**.
    
4.  Press the  **"Edit"**  button in the top right.
    
5.  Update the website, username, or password.
    
6.  **Confirm**  to save changes.
    

----------

## Hardware

-   **LilyGO T-Deck**
    
    -   **ESP32-S3FN16R8**  dual-core LX7 microprocessor
        
    -   2.8-inch, 320×240 pixel IPS LCD  **display**
        
    -   **Mini keyboard**
        
-   **SD card**

