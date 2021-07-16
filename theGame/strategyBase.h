#ifndef strategyBase_h
#define strategyBase_h

#include <lists.h>

#define DAY_MS       24000
#define NUM_NATIONS  3

enum  mediumType     { surface, water, air };
enum  territoryType  { land, river, sea };
enum  unitType       { rawMaterial, supplies, soldier, truck,  tank, fighter, bomber, freighter, submarine,  destroyer, factory, ACCarrier, battelship };
enum  nationType     { neutralNation, redNation, blueNation };



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
   virtual  float storedValue();
   virtual  float incomeRate();
   virtual  void  addDefenders(unitSet* inDefenders);
   virtual  void  removeDefenders(unitSet* inDefenders);
   
            territoryType  TType;
            unitSet*       defendingUnits;
};

class gameMap {

    public:
            gameMap(int inNumTerritories);
   virtual  ~gameMap(void);

   virtual  setupTerritories(void);
   
            territory* territoryArray;
};

#endif
