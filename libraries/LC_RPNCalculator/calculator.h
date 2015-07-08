//
//  calculator.h
//  Mr Snorts
//
//  Created by Jim Lee on 5/12/15.
//  Copyright (c) 2015 ___THEMADHOUSE___. All rights reserved.
//

#ifndef Mr_Snorts_calculator_h
#define Mr_Snorts_calculator_h

#define NUM_STO_REG 10

class calculator {
	
public:
	calculator(void);
	~calculator(void);
	
	void    buttonClick(const char* buttonID);
    double  getX(void);
	double  getY(void);
	double  getZ(void);
	double  getT(void);
	double	getStoVal(int i);
	void	setDegrees(bool inDegrees);
	bool	toggleDegrees(void);
	bool	getDegrees(void);
    double  rad_2_deg(double x);
    double  deg_2_rad(double x);
	int		getFixVal(void);
	bool	getFix();
    bool	getSto();
    bool	getRcl();
    bool	getEnter();
	bool	isEditing(void);
	
protected:
	void    startEditing(void);
	void    handleNumber(int inNum);
    void    handlePoint(void);
    void    handleClx(void);
	void    handleRollDown(void);
	void    pushStack(void);
	void    popStack(void);
	void    xyFunction(double result);
	void    xFunction(double result);

	
	double  t;          // Like the old HP11C
	double  z;
	double  y;
	double	x;
    double  sto[NUM_STO_REG];   // 10 storage registers 0..9
	int		fix;
    bool    editing;
    bool    point;
	bool	degrees;
	bool	enter;
	bool	fixPressed;
    bool    stoPressed;
    bool    rclPressed;
    double  multiplyer;
};

#endif
