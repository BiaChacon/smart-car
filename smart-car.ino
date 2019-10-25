#include <Fuzzy.h>

#include <Ultrasonic.h>
#include <Fuzzy.h>
#include <FuzzySet.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>

int inMotorA1=2, inMotorA2=3;
int inMotorB1=4, inMotorB2=5;
float velocidadeMotor1=10;
float velocidadeMotor2=11;

#define pino_trigger 7
#define pino_echo 6
Ultrasonic ultrasonic(pino_trigger, pino_echo);
Fuzzy* fuzzy = new Fuzzy();
 
void setup(){
  
  pinMode(velocidadeMotor1,OUTPUT);
  pinMode(velocidadeMotor2,OUTPUT);

  pinMode(inMotorA1,OUTPUT);
  pinMode(inMotorB1,OUTPUT);
  pinMode(inMotorA2,OUTPUT);
  pinMode(inMotorB2,OUTPUT);
  
  Serial.begin(9600);
  
  //Criando o FuzzyInput distância
  FuzzyInput* distancia = new FuzzyInput(1);
  
  //Criando os conjuntos fuzzy que representam a distância
  //Conjunto distância pequena
  FuzzySet* pequena = new FuzzySet(5, 20, 20, 40);
  distancia->addFuzzySet(pequena);
  //Conjunto distância média
  FuzzySet* media = new FuzzySet(30, 50, 50, 70);
  distancia->addFuzzySet(media);
  //Conjunto distância grande
  FuzzySet* grande = new FuzzySet(60, 100, 100, 200);
  distancia->addFuzzySet(grande);
  //Adicionando a entrada fuzzy ao sistema
  fuzzy->addFuzzyInput(distancia); 
  
  //Criando o FuzzyOutput velocidade
  FuzzyOutput* velocidade = new FuzzyOutput(1);
  
  //Criando os conjuntos fuzzy que representam a velocidade
  //Conjunto velocidade lenta
  FuzzySet* lenta = new FuzzySet(50, 50, 100, 150);
  velocidade->addFuzzySet(lenta);
  //Conjunto velocidade normal
  FuzzySet* normal = new FuzzySet(75, 125, 125, 180);
  velocidade->addFuzzySet(normal);
  //Conjunto velocidade rapida
  FuzzySet* rapida = new FuzzySet(150, 200, 200, 255);
  velocidade->addFuzzySet(rapida);
  // Adicionando o FuzzyOutput velocidade ao sistema
  fuzzy->addFuzzyOutput(velocidade); 
  
  //Montando as regras Fuzzy
  // FuzzyRule "SE distancia = pequena ENTÃO velocidade = lenta"
  FuzzyRuleAntecedent* ifDistanciaPequena = new FuzzyRuleAntecedent();
  ifDistanciaPequena->joinSingle(pequena);
  FuzzyRuleConsequent* thenVelocidadeLenta = new FuzzyRuleConsequent();
  thenVelocidadeLenta->addOutput(lenta);
  
  //Criando uma regra (FuzzyRule) a partir do antecedente e do consequente
  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifDistanciaPequena, thenVelocidadeLenta);
  //Adicionando a regra ao sistema
  fuzzy->addFuzzyRule(fuzzyRule01); 
  
  // FuzzyRule "SE distancia = segura ENTÃO velocidade = normal"
  FuzzyRuleAntecedent* ifDistanciaMedia = new FuzzyRuleAntecedent();
  ifDistanciaMedia->joinSingle(media);
  FuzzyRuleConsequent* thenVelocidadeNormal = new FuzzyRuleConsequent();
  thenVelocidadeNormal ->addOutput(normal);
  FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifDistanciaMedia, thenVelocidadeNormal);
  // Adicionando a regra ao sistema
  fuzzy->addFuzzyRule(fuzzyRule02); 
  
  // FuzzyRule "SE distancia = grande ENTÃO velocidade = alta"
  FuzzyRuleAntecedent* ifDistanciaGrande = new FuzzyRuleAntecedent();
  ifDistanciaGrande->joinSingle(grande);
  FuzzyRuleConsequent* thenVelocidadeRapida = new FuzzyRuleConsequent();
  thenVelocidadeRapida->addOutput(rapida);
  FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifDistanciaGrande, thenVelocidadeRapida);
  // Adicionando o FuzzyRule ao objeto Fuzzy
  fuzzy->addFuzzyRule(fuzzyRule03); 

}
 
void loop(){
  
    long microsec = ultrasonic.timing();
    
    // Informando o valor da entrada (distância), passando seu ID e valor
    float d = ultrasonic.convert(microsec, Ultrasonic::CM);
  
    fuzzy->setInput(1, d);
    
    // Executando a fuzzyficação
    fuzzy->fuzzify();
    
    float v = fuzzy->defuzzify(1);

    analogWrite(velocidadeMotor1,v);
    analogWrite(velocidadeMotor2,v);
    
    digitalWrite(inMotorA1,LOW);
    digitalWrite(inMotorA2,HIGH);
    digitalWrite(inMotorB1,HIGH);
    digitalWrite(inMotorB2,LOW);
    
    Serial.print("Distancia = ");
    Serial.print(d);
    Serial.print(" Velocidade = ");
    Serial.println(v);
    
  //delay(1000);

}
