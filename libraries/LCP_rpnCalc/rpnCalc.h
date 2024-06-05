#ifndef rpnCalc_h
#define rpnCalc_h

#include <label.h>
#include <calculator.h>
#include <lilOS.h>
#include <calcButton.h>



class rpnCalc : public panel {

	public:
				rpnCalc(int panelID);
	virtual	~rpnCalc(void);

				void	setupButtonColors(void);
	virtual	void	setup(void);
	virtual	void	loop(void);
				
				void	buttonClick(const char* btnID);
				bool	getDegrees(void);
  				void	toggleDegrees(void);
  				void	toggle2ndFx(void);
  				
				calculator	calc;           // The calculator object. Feed it key strokes and it gives calculations.
				label*		XReg;
				bool			secondFx; 
				bool			btnPressed;
				colorObj		numberActiveBColor;
				colorObj		numberActiveTColor;
				colorObj		numberClickedBColor;
				colorObj		numberClickedTColor;

				colorObj		editActiveBColor;
				colorObj		editActiveTColor;
				colorObj		editClickedBColor;
				colorObj		editClickedTColor;

				colorObj		fxActiveBColor;
				colorObj		fxActiveTColor;
				colorObj		fxClickedBColor;
				colorObj		fxClickedTColor;
};


#endif
