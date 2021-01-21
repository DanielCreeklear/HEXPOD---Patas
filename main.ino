/* ------------- MOVIMENTO PATAS VERSAO 2.3.5 ------------- 
 *  Desenvolvedores: Daniel Lopes / Enrique Emanuel
 *  ETEC Martin Luther King
 *  Sao Paulo(SP), Brasil - 2019
 *  Contatos: ddanielssoares@gmail.com
 *  enriqueemanuelcontato@gmail.com
 *  
 *  CORE HEXPOD: V2.35
 *  CONTROLE HEXPOD: V1.3
 *  
 *  =========Log de atualizacoes=============
 *  CONTEUDO NOVO:
 *  - Portas para controlar o LED e a Camera.
 *  - Acrescentado funcoes aos botoes S, Z, C, G e B sendo:
 *  S: Movimentacao autonoma
 *  G: Liga/Desliga LED
 *  B: Liga/Desliga Camera
 *  - Criada funcao test(), que executa apenas o front(). 
 *  - Criada funcao controle(), que executa todo o funcionamento do robo atraves do controle.
 * CONTEUDO MODIFICADO: 
 * - Valor da constante 'delay1' mudado para 650
 * CONTEUDO EXCLUIDO:
 * - Biblioteca Software Serial e todas suas linhas de comandos
 * - Função bateria()
 * 
 */
//Bibliotecas
#include <VirtualWire.h>
#include <VirtualWire_Config.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>



Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);  //Seleciona o shield com endereco 0x40

//Configuração PWM para o servo motor.
#define SERVOMIN  150  //Comprimento de pulso minimo
#define SERVOMAX  600  //Comprimento de pulso maximo

//Porta dos Sensores
uint8_t motores[6] = {2, 3, 4, 5, 6, 7 }; //array que define os pinos dos motores
#define sensorA 2
#define sensorB 3
#define sensorC 4
#define sensorD 5
#define sensorE 6
#define sensorF 7
#define pinRF  12
#define pinLED 13
boolean LED = 0;
boolean CAMERA = 0;
struct tipoPacote {
  char valor1;
};
tipoPacote pacote; 
uint8_t buf[sizeof(pacote)];
uint8_t buflen = sizeof(pacote);
//Delays para movimento das patas
  //4V:
  #define delay1 650 //Delay para iniciar o lado B
  #define delay2 100 //Delay para parar o lado A
  //5V:
  #define delay3 900 //Delay para iniciar o lado B
  #define delay4 100 //Delay para parar o lado A
  //6V:
  #define delay5 700 //Delay para iniciar o lado B
  #define delay6 100 //Delay para parar o lado A
  //7V:
  #define delay7 600 //Delay para iniciar o lado B
  #define delay8 100 //Delay para parar o lado A

//Declarando Funções que serão utilizadas:
void front_auto();
int  graus(int x);
void front();
void right();  
void left();
void parado();
void controle();
void test();

void setup() {
  pwm.begin(); //Inicia o controle dos servos(PWM)
  pwm.setPWMFreq(50); //Frequencia de comunicaçao com o driver em 50Hz

  for(int i = 0; i<7; i++) pinMode(motores[i] , INPUT_PULLUP); //Portas 2 - 8 como entradas com resistor Pullup
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  vw_set_rx_pin(pinRF);
  vw_setup(2000);   
  vw_rx_start();
}

void loop() {
  controle();
}

void controle(){
  if (vw_have_message()){
   vw_get_message(buf, &buflen);   
   memcpy(&pacote,&buf,buflen);
    switch (pacote.valor1) {
          case 'W':
            front();
          break;

          case 'S':
            front_auto();
          break;
          
          case 'A':
            left();
          break;
  
          case 'D':
            right();
          break;
  
          case 'C':
            parado(0);
          break;
          
          case'G':
            LED = !LED;
            digitalWrite(8, LED);
          break;

          case'B':
            CAMERA = !CAMERA;
            digitalWrite(9, CAMERA);
          break; 
    }
  }
}

void test(){
  front();
}
void front(){     //Funcao que faz o Hexpod andar para frente
  pwm.setPWM(0, 0, graus(180));     //Motor 0 gira com sentido para frente no robo 
  pwm.setPWM(4, 0, graus(0));       //Motor 4(motor oposto) gira com sentido para frente no robo
  pwm.setPWM(2, 0, graus(180));     //Motor 2 gira com sentido para frente no robo
  delay(delay1);                       //Delay para iniciar o lado B
  pwm.setPWM(3, 0, graus(0));       //Motor 3 gira com sentido para frente no robo
  pwm.setPWM(1, 0, graus(180));     //Motor 1(motor oposto) gira com sentido para frente no robo
  pwm.setPWM(5, 0, graus(0));       //Motor 5 gira com sentido para frente no robo
  delay(delay2);                       //Delay para parar o lado A
  pwm.setPWM(0, 0, graus(51));      //Motor 0 parado
  pwm.setPWM(4, 0, graus(56));      //Motor 4 parado
  pwm.setPWM(2, 0, graus(51));      //Motor 2 parado
  int a = delay1 - delay2;
  delay(a);                       //Delay para parar o lado B
  pwm.setPWM(3, 0, graus(51));      //Motor 3 parado
  pwm.setPWM(1, 0, graus(51));      //Motor 1 parado
  pwm.setPWM(5, 0, graus(51));      //Motor 5 parado
  //delay(10);                        //Delay apenas para diminuir a velocidade do processamento
  parado(0);                     //Chama a funaao parado que sincroniza as patas ao chao, com sentido para frente do robo
}

void right(){
  pwm.setPWM(0, 0, graus(180));     //Motor 0 gira com sentido para frente no robo 
  pwm.setPWM(4, 0, graus(0));       //Motor 4(motor oposto) gira com sentido para frente no robo
  pwm.setPWM(2, 0, graus(180));     //Motor 2 gira com sentido para frente no robo
  delay(delay1);                       //Delay para iniciar o lado B
  pwm.setPWM(3, 0, graus(180));       //Motor 3 gira com sentido para frente no robo
  pwm.setPWM(1, 0, graus(0));     //Motor 1(motor oposto) gira com sentido para frente no robo
  pwm.setPWM(5, 0, graus(180));       //Motor 5 gira com sentido para frente no robo
  delay(delay2);                       //Delay para parar o lado A
  pwm.setPWM(0, 0, graus(51));      //Motor 0 parado
  pwm.setPWM(4, 0, graus(51));      //Motor 4 parado
  pwm.setPWM(2, 0, graus(51));      //Motor 2 parado
  int a = delay1 - delay2;
  delay(a);   
  pwm.setPWM(3, 0, graus(51));      //Motor 3 parado
  pwm.setPWM(1, 0, graus(51));      //Motor 1 parado
  pwm.setPWM(5, 0, graus(51));      //Motor 5 parado
  //delay(10);                        //Delay apenas para diminuir a velocidade do processamento
  parado(1);                        //Chama a funaao parado que sincroniza as patas ao chao, com sentido para direita do robo
}

void left(){     //Funcao que faz o Hexpod andar para frente
  pwm.setPWM(0, 0, graus(0));     //Motor 0 gira com sentido para frente no robo 
  pwm.setPWM(4, 0, graus(180));       //Motor 4(motor oposto) gira com sentido para frente no robo
  pwm.setPWM(2, 0, graus(0));     //Motor 2 gira com sentido para frente no robo
  delay(delay1);                       //Delay para iniciar o lado B
  pwm.setPWM(3, 0, graus(0));       //Motor 3 gira com sentido para frente no robo
  pwm.setPWM(1, 0, graus(180));     //Motor 1(motor oposto) gira com sentido para frente no robo
  pwm.setPWM(5, 0, graus(0));       //Motor 5 gira com sentido para frente no robo
  delay(delay2);                       //Delay para parar o lado A
  pwm.setPWM(0, 0, graus(51));      //Motor 0 parado
  pwm.setPWM(4, 0, graus(51));      //Motor 4 parado
  pwm.setPWM(2, 0, graus(51));      //Motor 2 parado
  int a = delay1 - delay2;
  delay(a);   
  pwm.setPWM(3, 0, graus(51));      //Motor 3 parado
  pwm.setPWM(1, 0, graus(51));      //Motor 1 parado
  pwm.setPWM(5, 0, graus(51));      //Motor 5 parado
  //delay(10);                        //Delay apenas para diminuir a velocidade do processamento
  parado(2);                     //Chama a funaao parado que sincroniza as patas ao chao, com sentido para esquerda do robo
}

void parado(int sentido){       //Funcao que sincroniza as patas no chao, recebe valor: sentido de rotacao, para sincronizar
  if(sentido == 0) {                    //Se sentido for 0, sincroniza as patas para tras do robo


      //Sincronizar Motor 2:  
      if(!digitalRead(sensorB)){    //Se o sensor B, localizado na porta 3, estiver ativado
        pwm.setPWM(1, 0, graus(51));//Motor 1 parado
      } else {                      //Se não
          do{                       //Executa uma vez o que estiver dentro de "do"
            pwm.setPWM(1, 0, graus(180));//Motor 1 gira com sentido para frente no robo 
          }while(digitalRead(sensorB));//Enquanto sensor B estiver desativado ele executa "do"
          if(!digitalRead(sensorB)){//Se o sensor B, localizado na porta 3, estiver ativado
            pwm.setPWM(1, 0, graus(51));//Motor 1 parado
          }                         //
      }                             //



      //Sincronizar Motor 5:
      if(!digitalRead(sensorE)){
        pwm.setPWM(4, 0, graus(56));
      } else {
          do{
            pwm.setPWM(4, 0, graus(0));
          } while(digitalRead(sensorE));
          if(!digitalRead(sensorE)){
            pwm.setPWM(4, 0, graus(56));
          }
      }



      //Sincronizar Motor 6:
      if(!digitalRead(sensorF)){
        pwm.setPWM(5, 0, graus(51));
      } else {
          do{
            pwm.setPWM(5, 0, graus(0));
          }while(digitalRead(sensorF));
          if(!digitalRead(sensorF)){
            pwm.setPWM(5, 0, graus(51));
          }
      }



      //Sincronizar Motor 3:
      if(!digitalRead(sensorC)){    //Se o sensor C, localizado na porta 4, estiver ativado
        pwm.setPWM(2, 0, graus(51));//Motor 2 parado
      } else {                      //Se não
          do{                       //Executa uma vez o que estiver dentro de "do"
            pwm.setPWM(2, 0, graus(180));//Motor 2 gira com sentido para frente no robo
          }while(digitalRead(sensorC));//Enquanto sensor B estiver desativado ele executa "do"
          if(!digitalRead(sensorC)){//Se o sensor C, localizado na porta 4, estiver ativado
            pwm.setPWM(2, 0, graus(51));//Motor 2 parado
          }                         //
      }                             //


      
    //Sincronizar Motor 1:
      if(!digitalRead(sensorA)){    //Se o sensor A, localizado na porta 2, estiver ativado
        pwm.setPWM(0, 0, graus(51));//Motor 0 parado
      } else {                      //Se não
          do {                      //Executa uma vez o que estiver dentro de "do"
            pwm.setPWM(0, 0, graus(180));//Motor 0 gira com sentido para frente no robo 
          } while(digitalRead(sensorA));//Enquanto sensor A estiver desativado ele executa "do"
          if(!digitalRead(sensorA)){//Se o sensor A, localizado na porta 2, estiver ativado
            pwm.setPWM(0, 0, graus(51));//Motor 0 parado
          }                         //
      }                             //


      
    //Sincronizar Motor 4:               
      if(!digitalRead(sensorD)){
        pwm.setPWM(3, 0, graus(51));
      } else {
          do{
            pwm.setPWM(3, 0, graus(0));
          }while(digitalRead(sensorD));
          if(!digitalRead(sensorD)){
            pwm.setPWM(3, 0, graus(51));
          }
      }
  }//end if
  if(sentido == 1) {                    //Se sentido for 1, sincroniza as patas para direita do robo


    //Sincronizar Motor 2:
      if(!digitalRead(sensorB)){    //Se o sensor B, localizado na porta 3, estiver ativado
        pwm.setPWM(1, 0, graus(51));//Motor 1 parado
      } else {                      //Se não
          do{                       //Executa uma vez o que estiver dentro de "do"
            pwm.setPWM(1, 0, graus(0));//Motor 1 gira com sentido para tras no robo 
          }while(digitalRead(sensorB));//Enquanto sensor B estiver desativado ele executa "do"
          if(!digitalRead(sensorB)){//Se o sensor B, localizado na porta 3, estiver ativado
            pwm.setPWM(1, 0, graus(51));//Motor 1 parado
          }                         //
      }                             //


      //Sincronizar Motor 5:
      if(!digitalRead(sensorE)){
        pwm.setPWM(4, 0, graus(51));
      } else {
          do{
            pwm.setPWM(4, 0, graus(0));
          } while(digitalRead(sensorE));
          if(!digitalRead(sensorE)){
            pwm.setPWM(4, 0, graus(51));
          }
      }


      //Sincronizar Motor 6:
      if(!digitalRead(sensorF)){
        pwm.setPWM(5, 0, graus(51));
      } else {
          do{
            pwm.setPWM(5, 0, graus(180));
          }while(digitalRead(sensorF));
          if(!digitalRead(sensorF)){
            pwm.setPWM(5, 0, graus(51));
          }
      }


      //Sincronizar Motor 3:
      if(!digitalRead(sensorC)){    //Se o sensor C, localizado na porta 4, estiver ativado
        pwm.setPWM(2, 0, graus(51));//Motor 2 parado
      } else {                      //Se não
          do{                       //Executa uma vez o que estiver dentro de "do"
            pwm.setPWM(2, 0, graus(180));//Motor 2 gira com sentido para frente no robo
          }while(digitalRead(sensorC));//Enquanto sensor B estiver desativado ele executa "do"
          if(!digitalRead(sensorC)){//Se o sensor C, localizado na porta 4, estiver ativado
            pwm.setPWM(2, 0, graus(51));//Motor 2 parado
          }                         //
      }                             //


      
    //Sincronizar Motor 1:
      if(!digitalRead(sensorA)){    //Se o sensor A, localizado na porta 2, estiver ativado
        pwm.setPWM(0, 0, graus(51));//Motor 0 parado
      } else {                      //Se não
          do {                      //Executa uma vez o que estiver dentro de "do"
            pwm.setPWM(0, 0, graus(180));//Motor 0 gira com sentido para frente no robo 
          } while(digitalRead(sensorA));//Enquanto sensor A estiver desativado ele executa "do"
          if(!digitalRead(sensorA)){//Se o sensor A, localizado na porta 2, estiver ativado
            pwm.setPWM(0, 0, graus(51));//Motor 0 parado
          }                         //
      }                             //

      
      
    //Sincronizar Motor 4:
      if(!digitalRead(sensorD)){
        pwm.setPWM(3, 0, graus(51));
      } else {
          do{
            pwm.setPWM(3, 0, graus(180));
          }while(digitalRead(sensorD));
          if(!digitalRead(sensorD)){
            pwm.setPWM(3, 0, graus(51));
          }
      }
  }//end if
  
  if(sentido == 3) {                    //Se sentido for 0, sincroniza as patas para esquerda do robo


    //Sincronizar Motor 2:
      if(!digitalRead(sensorB)){    //Se o sensor B, localizado na porta 3, estiver ativado
        pwm.setPWM(1, 0, graus(51));//Motor 1 parado
      } else {                      //Se não
          do{                       //Executa uma vez o que estiver dentro de "do"
            pwm.setPWM(1, 0, graus(180));//Motor 1 gira com sentido para frente no robo 
          }while(digitalRead(sensorB));//Enquanto sensor B estiver desativado ele executa "do"
          if(!digitalRead(sensorB)){//Se o sensor B, localizado na porta 3, estiver ativado
            pwm.setPWM(1, 0, graus(51));//Motor 1 parado
          }                         //
      }                             //


      //Sincronizar Motor 5:
      if(!digitalRead(sensorE)){
        pwm.setPWM(4, 0, graus(51));
      } else {
          do{
            pwm.setPWM(4, 0, graus(180));
          } while(digitalRead(sensorE));
          if(!digitalRead(sensorE)){
            pwm.setPWM(4, 0, graus(51));
          }
      }


      //Sincronizar Motor 6:
      if(!digitalRead(sensorF)){
        pwm.setPWM(5, 0, graus(51));
      } else {
          do{
            pwm.setPWM(5, 0, graus(0));
          }while(digitalRead(sensorF));
          if(!digitalRead(sensorF)){
            pwm.setPWM(5, 0, graus(51));
          }
      }



  //Sincronizar Motor 3:
      if(!digitalRead(sensorC)){    //Se o sensor C, localizado na porta 4, estiver ativado
        pwm.setPWM(2, 0, graus(51));//Motor 2 parado
      } else {                      //Se não
          do{                       //Executa uma vez o que estiver dentro de "do"
            pwm.setPWM(2, 0, graus(0));//Motor 2 gira com sentido para tras no robo
          }while(digitalRead(sensorC));//Enquanto sensor B estiver desativado ele executa "do"
          if(!digitalRead(sensorC)){//Se o sensor C, localizado na porta 4, estiver ativado
            pwm.setPWM(2, 0, graus(51));//Motor 2 parado
          }                         //
      }                             //



    //Sincronizar Motor 1:
      if(!digitalRead(sensorA)){    //Se o sensor A, localizado na porta 2, estiver ativado
        pwm.setPWM(0, 0, graus(51));//Motor 0 parado
      } else {                      //Se não
          do {                      //Executa uma vez o que estiver dentro de "do"
            pwm.setPWM(0, 0, graus(0));//Motor 0 gira com sentido para tras no robo 
          } while(digitalRead(sensorA));//Enquanto sensor A estiver desativado ele executa "do"
          if(!digitalRead(sensorA)){//Se o sensor A, localizado na porta 2, estiver ativado
            pwm.setPWM(0, 0, graus(51));//Motor 0 parado
          }                         //
      }                             //


      
    //Sincronizar Motor 4:
      if(!digitalRead(sensorD)){
        pwm.setPWM(3, 0, graus(51));
      } else {
          do{
            pwm.setPWM(3, 0, graus(0));
          }while(digitalRead(sensorD));
          if(!digitalRead(sensorD)){
            pwm.setPWM(3, 0, graus(51));
          }
      }
  }//end if
}//end void parado
  
int graus(int x) { //Funçao 'graus' que recebe um valor Inteiro armazenado em 'x'                                                            
  int graus = x; //Variavel inteira 'graus' recebe o valor de 'x'
  graus = map(graus, 0, 180, SERVOMIN, SERVOMAX); //Regra de 3, para converter graus no valor do PWM
  
  return graus; //Retorna como resultado da funcao, o valor da variavel 'graus'
}

void front_auto(){
  
}
