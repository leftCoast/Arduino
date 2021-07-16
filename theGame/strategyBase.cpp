#include <strategyBase.h>
#include <resizeBuff.h>


class nation {

    public:
            nation(nationType inNation);
   virtual  ~nation(void);

            void     begin(void);
            unitSet  createSet();
            bool     attackTerritory(territory* inTrritory,unitSet* attackingForce);       
};

nation nations[NUM_NATIONS];

class gameUnit : public  linkListObj {

   public:
            gameUnit(unitType inType,nation* inNation=NULL);
   virtual  ~gameUnit(void);

   virtual  float    unitCost(void);
   virtual  float    attackValue(void);
   virtual  float    defendValue(void);
   virtual  medium   ourMedium(void);
   virtual  bool     moveUnit(nationType by,territory* from,territory* to);
   
            unitType ourType;
            nation*  ourNation;        
};


class unitSet :   public linkList {

   public:
            unitSet(void);
   virtual  ~unitSet(void);

            void  addUnit(gameUnit* unitPtr);
            void  releaseUnit(gameUnit* unitPtr);
            void  setMode(bool deploy);
            bool  moveSet(nationType by,territory* to);
            void  attack(unitSet* defending);
            void  defend(unitSet* attacking);

            bool  deployed;
};


class territory {

   public:
            territory();
   virtual  ~territory(void);

            void  setType(territoryType inType);
   virtual  void  addAdjacent(territory* neighbor);
   virtual  float storedValue();
   virtual  float incomeRate();
   virtual  void  addDefenders(unitSet* inDefenders);
   virtual  void  removeDefenders(unitSet* inDefenders);
   virtual  void  addAdjacent(territory* neighbor);
   
            territoryType  TType;
            unitSet*       defendingUnits;
};



// ******************************
// class gameMap {
// ******************************


gameMap::gameMap(int inNumTerritories) {

   numTerritories = inNumTerritories;
   territoryList = NULL;
   resizeBuff(numTerritories*sizeof(territory),&territoryList);
   }

   
gameMap::~gameMap(void) {
      
      resizeBuff(0,&territoryList);
   }

void gameMap::setupTerritory(int TerritoryID) {

   territory*  TPtr;
   
   TPtr = &(territoryList[TerritoryID]);
   switch(TerritoryID) {
   case 1 :
      TPtr->setType(sea);
      TPtr->addAdjacent(&territoryList[2]);
      TPtr->addAdjacent(&territoryList[3]);
      TPtr->addAdjacent(&territoryList[4]);
      TPtr->addAdjacent(&territoryList[5]);
   }
}


void gameMap::setupTerritories(void) {

   if (territoryList) {
      for (int i=0;i<numTerritories;i++) {
         setupTerritory(i+1);                // Array is base zero, maps are base 1.
      }
   }
