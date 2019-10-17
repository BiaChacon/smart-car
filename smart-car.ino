#include <Ultrasonic.h>
#include <Fuzzy.h>
#include <FuzzySet.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>

#define pino_trigger 2
#define pino_echo 3
Ultrasonic ultrasonic(pino_trigger, pino_echo);
Fuzzy* fuzzy = new Fuzzy();
 
void setup(){
  
  Serial.begin(9600);
  
  //Criando o FuzzyInput distância
  FuzzyInput* distancia = new FuzzyInput(1);
  
  //Criando os conjuntos fuzzy que representam a distância
  //Conjunto distância pequena
  FuzzySet* pequena = new FuzzySet(0, 10, 10, 20);
  distancia->addFuzzySet(pequena);
  //Conjunto distância média
  FuzzySet* media = new FuzzySet(20, 30, 50, 70);
  distancia->addFuzzySet(media);
  //Conjunto distância grande
  FuzzySet* grande = new FuzzySet(80, 100, 100, 100);
  distancia->addFuzzySet(grande);
  //Adicionando a entrada fuzzy ao sistema
  fuzzy->addFuzzyInput(distancia); 
  
  //Criando o FuzzyOutput velocidade
  FuzzyOutput* velocidade = new FuzzyOutput(1);
  
  //Criando os conjuntos fuzzy que representam a velocidade
  //Conjunto velocidade lenta
  FuzzySet* lenta = new FuzzySet(0, 5, 5, 10);
  velocidade->addFuzzySet(lenta);
  //Conjunto velocidade normal
  FuzzySet* normal = new FuzzySet(5, 10, 20, 40);
  velocidade->addFuzzySet(normal);
  //Conjunto velocidade rapida
  FuzzySet* rapida = new FuzzySet(30, 40, 40, 50);
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
    
    Serial.print("Distancia = ");
    Serial.print(d);
    Serial.print(" Velocidade = ");
    Serial.println(v);
    
   delay(1000);

}
