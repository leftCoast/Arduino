#include <triDBase.h>
#include <SD.h>

int8_t STLHeader[80];
int8_t numFacets;

  
void readFacet(File* inFile) {

	float		vect[3];
	float		vertex[3];
	uint8_t	extra[2];
	
	inFile->read((void*)&vect,12);
	Serial.print("Vect angles x,y,z     : ");
	Serial.print(vect[0]);Serial.print(", ");
	Serial.print(vect[1]);Serial.print(", ");
	Serial.print(vect[2]);Serial.println();
	
	inFile->read((void*)&vertex,12);
	Serial.print("Vertex 1 points x,y,z : ");
	Serial.print(vertex[0]);Serial.print(", ");
	Serial.print(vertex[1]);Serial.print(", ");
	Serial.print(vertex[2]);Serial.println();
	
	inFile->read((void*)&vertex,12);
	Serial.print("Vertex 2 points x,y,z : ");
	Serial.print(vertex[0]);Serial.print(", ");
	Serial.print(vertex[1]);Serial.print(", ");
	Serial.print(vertex[2]);Serial.println();
	
	inFile->read((void*)&vertex,12);
	Serial.print("Vertex 3 points x,y,z : ");
	Serial.print(vertex[0]);Serial.print(", ");
	Serial.print(vertex[1]);Serial.print(", ");
	Serial.print(vertex[2]);Serial.println();
	
	inFile->read((void*)&extra,2);
	Serial.print("Extra bytes a & b     : ");
	Serial.print(extra[0]);Serial.print(", ");
	Serial.print(extra[1]);Serial.println();
}

  
void printSTLFile(const char* filePath) {

	File	theSTLfile;
	char	buff[81];
	long	facets;
	char*	strEnd;
	
	theSTLfile = SD.open(filePath,FILE_READ);
	if (theSTLfile) {
		theSTLfile.read(buff,80);
		strEnd = strstr(buff,"   ");
		if (strEnd) {
			strEnd[0] = '\0';
			Serial.println(buff);
		
			theSTLfile.read((char*)&facets,4);
			Serial.println(facets);
			
			for(int i=0;i<facets;i++) {
				readFacet(&theSTLfile);
				Serial.println("---------------------------------------");
			}
		}
		theSTLfile.close();
	}
}

