#include <Wire.h>              
#include <LiquidCrystal_I2C.h> 
#include<Stepper.h>
#define TABLE_SIZE 15

typedef struct Item {
    int id;
    char name[50];
    float price;
    int stock; 
    struct Item* next;
} Item;

//lcd 
LiquidCrystal_I2C lcd(0x27, 16, 2);

//distance sensor
const unsigned int TRIG_PIN=52;
const unsigned int ECHO_PIN=50;
const  unsigned int BAUD_RATE=9600;

//motor
Stepper motor(300,8,9,10,11);
int length=1000;
int speed=200;

void setup() {
  lcd.init();
  lcd.backlight();
  //distance sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(BAUD_RATE);
  //motor
  motor.setSpeed(speed); //ppm
  //motor.step(length);

  //set up the database
  Item* hashTable[TABLE_SIZE];
  for (int i = 0; i < TABLE_SIZE; i++) {
    hashTable[i] = NULL;
  }
  insertItem(hashTable,1, "biscoff", 3, 100);
  insertItem(hashTable,2, "butter_cheese", 5, 100);
  insertItem(hashTable,3, "cadina", 7, 100);
  insertItem(hashTable,4, "crispiroll",3, 100);
  insertItem(hashTable,5, "hokkaido", 5, 100);
  insertItem(hashTable,6, "nothing", 0, 100);
  insertItem(hashTable,7, "onion", 3, 100);
  insertItem(hashTable,8, "scientist_noodles", 3, 100);
  insertItem(hashTable,9, "snapu_cookies", 5, 100);
  insertItem(hashTable,10, "Soda", 5, 100);
}

void loop() {

}

void print_lcd(char* name,int price,int stock){
  lcd.setCursor(0, 0); 
  lcd.print(name);
  lcd.setCursor(0, 1);
  lcd.print($ );
  lcd.print(price); 
  lcd.print(", ");
  lcd.print(stock);
}

void move(int length){
  motor.step(length);
}

float get_distance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN,  HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  const unsigned long duration= pulseIn(ECHO_PIN, HIGH);
  float distance= duration/29/2;
  if(duration==0){
    Serial.println("Warning: no pulse from sensor");
    }  
  else{
    Serial.print("distance to nearest object:");
    Serial.println(distance);
    Serial.println(" cm");
    return distance;
  }
}

// hash
unsigned int hash(const int id) {
    return id % TABLE_SIZE;
}

void insertItem(Item* hashTable[],int id,const char* name, float price, int stock) {
    unsigned int index = hash(id);

    while (hashTable[index] != NULL) {
        index = (index + 1) % TABLE_SIZE;
    }

    Item* newItem = (Item*)malloc(sizeof(Item));
    newItem->id = id;
    strcpy(newItem->name, name);
    newItem->price = price;
    newItem->stock = stock;
    newItem->next = (hashTable[index]);  // insert to the head
    hashTable[index] = newItem;
}

Item* findItem(Item* hashTable[],const int id) {
    unsigned int index = hash(id);
    Item* item = hashTable[index];
    while (item != NULL) {
        if (item->id == id) {
            return item;
        }
        item = item->next;
    }
    return NULL;
}

void deleteItem(Item* hashTable[],int id) {
    unsigned int index = hash(id);
    Item* item = hashTable[index];
    Item* prev = NULL;

    while (item != NULL) {
        if (item->id == id) {
            if (prev == NULL) {
                hashTable[index] = item->next;
            } else {
                prev->next = item->next;
            }
            free(item);
            return;
        }
        prev = item;
        item = item->next;
    }
    printf("Item with ID %d not found\n", id);
}


