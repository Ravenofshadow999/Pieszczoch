  /**************************************8
   * Program przyjmuje ramke danych: Fw1 w2 w3 w4 F\n
   * Ustawia dane w1-4 jako katy serwomechanizmow
   * sprawdza stan czujnikow CNY70 i wysyla pomiar
   * roboczo komunikacja szeregowa, pozniej po I2C
   * 
   * DODAC:
   * - funkcja pomiaru z czujnikow
   * - protokol I2C
   */
  
  
  
  
  #include <Servo.h>
  
  Servo serwo_1;  
  Servo serwo_2;
  Servo serwo_3;
  Servo serwo_4;

  int war[4]; //tablica na wartosci katow; war[0] -> wartosc dla serwo_1

  String bufor;


  // funkcja ustawiająca wartosc kata
void Set_angle(int *war)
{
  serwo_1.write(war[0]);
  serwo_2.write(war[1]);
  serwo_3.write(war[2]);
  serwo_4.write(war[3]);
}

void setup() {
  serwo_1.attach(3);  // setup serw
  serwo_2.attach(5);
  serwo_3.attach(6);
  serwo_4.attach(9);

  war[0] = 90;   // wartosci "zerowe"
  war[1] = 90;
  war[2] = 90;
  war[3] = 90;

  Serial.begin(9600);
  Serial.println("Hello!");
}

void loop() {
  
  /*odbierz ramke*/
  if(Serial.available())
  {
    bufor = Serial.readStringUntil('\n');

    /*rozszyfruj ramke*/
    int str_len = bufor.length() + 1; //zamiana stringa na tablice charow
    char char_bufor[str_len];
    bufor.toCharArray(char_bufor, str_len);

// F123 415 180 202 F
    
    if(char_bufor[0] == 'F' && char_bufor[bufor.length()-1] == 'F') //sprawdzenie poczatku i konca ramki
    {
      //szukanie spacji - rozdziela dane
      int i = 1;
      int j = 0;
      int w = 0; //do liczenia numeru przetwarzanej wartosci
      char *tablica;
      tablica = (char*) calloc(4, sizeof(*tablica)); //dynamiczne alokowanie kontenera - tablica 3 charow
      
      for(i; i < str_len; i++)  //bo pierwsza i ostatnia pozycja juz sprawdzona
      {
        
        if(char_bufor[i]!=' ')  //jesli dane pole nie jest spacja zapisuje je do kontenera
        {
          tablica[j] = char_bufor[i];
          j++;
        }

        else  //jesli znajdzie spacje konwertuje tablice charow do inta, usuwa tablice i deklaruje nowa
        {
          j = 0;
          war[w] = atoi(tablica);
          w++;
          free(tablica);
          tablica = NULL; //na wszelki wypadek
          if(w != 4)  //jesli nie przetworzyl 4 danych, stworz nowa tablice-kontener
          {
            tablica = (char*) calloc(4, sizeof(*tablica));
          }
          
        }//else
        
      }//for
      
    }//if

    //tu powinnam miec tablice intow war

    /*ustaw katy - Set_angle();*/
    //Set_angle(war);
    Serial.print("w1: ");
    Serial.println(war[0]);
    Serial.print("w2: ");
    Serial.println(war[1]);
    Serial.print("w3: ");
    Serial.println(war[2]);
    Serial.print("w4: ");
    Serial.println(war[3]);
    
    /*pobierz i wyslij wartosci z czujnikow*/

  
  }//if(Serial.available())
  
  delay(10);
}
