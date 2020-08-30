## Clothes Virtual fitting 
* This is Image Processing academic project that helps people in shopping .
* implemented using c++ and openCV
* uses face detection and pixel scanning

## Features
### In Release 1
* fit a T-shirt on your body
* View your final picture with the new T-shirt 

###How to use
* install openCV in your machine (we recommend  linux based OS )
  [use this link](https://github.com/AhmedKamal1432)
* Clone our Repository
* Open your terminal and Change Directory To our folder
* build the project by this command:

    ``` 
     g++ main.cpp -o exc.out `pkg-config opencv --cflags --libs`
    ```
* To run 

   ``` 
    ./exc.out your_photo.jpg T_shirt_photo.jpg 
   ```

###Sample input

<img src="https://raw.githubusercontent.com/AhmedKamal1432/ClothesVirtualFitting/master/images/input.jpg" alt="Drawing" style="width: 250px;"/>

### Sample Output
<img src="https://raw.githubusercontent.com/AhmedKamal1432/ClothesVirtualFitting/master/images/final.jpg" alt="Drawing" style="width: 250px;"/>

## Authors
* [Ahmed Kamal Abd El Raouf.](https://github.com/AhmedKamal1432)
* [Amr Ragab.](https://github.com/amrragab)

