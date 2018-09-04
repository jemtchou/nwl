#include "nwlConfigParser.hh"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

nwlConfigParser* nwlConfigParser::pInstance = 0;

nwlConfigParser::nwlConfigParser()
{ 
   fname="config"; 
   oerr.open("parser.log");
}

nwlConfigParser::~nwlConfigParser()
{ 
   oerr.close(); 
}

nwlConfigParser* nwlConfigParser::Instance()
{
  if(!pInstance) pInstance = new nwlConfigParser();
  return pInstance;  
}

ifstream nwlConfigParser::OpenFile()
{
  ifstream is (fname, ifstream::binary); // open file
  try{
   if (!is.is_open())
	throw std::runtime_error("Cannot open config file " + fname);;    
   }
  catch (const std::exception& e)
  {
    cerr << "Exception: " << e.what() << endl;
  }    
  return is;
}

//=========================================================================================================== NumberOfEvents 
long nwlConfigParser::GetNumberOfEvents(){
  ifstream is = OpenFile();

  long number;
  string CurStr;

  while(getline (is,CurStr)){

    if( CurStr.compare(0, 14, "NumberOfEvents")==0 ){
     
      
      string NumberOfEvents;
      size_t found = CurStr.find_first_of("#");
      if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
      found = CurStr.find_first_of(" ");
      
      if(found==string::npos){
	oerr <<"\n\nERROR: There is no any space in [NumberOfEvents] string" <<endl;
	return -1;
      }else{
	
	NumberOfEvents = CurStr.substr(found+1);
	while(NumberOfEvents.c_str()[0]==' '){ NumberOfEvents = NumberOfEvents.substr(1); }
	found = NumberOfEvents.find_first_of(" ");
	if(found!=string::npos){   NumberOfEvents = NumberOfEvents.substr(0, found); }

	number = atol(NumberOfEvents.c_str());

      }//end if(space)   
    }//end if(NumberOfEvents)
    
    CurStr.clear();
  }//end while()

  if (!is.eof()){

    oerr <<"\n\nERROR: config file is not read" <<endl;
    is.close();
    return -1;
  }//end if(eof)

  is.close();
  return number;
}






//============================================================================================================ LoggingType
string  nwlConfigParser::GetLoggingType(){

  ifstream is = OpenFile();

  string LoggingType;
  string CurStr;

  while(getline (is,CurStr)){
    
    if( CurStr.compare(0, 11, "LoggingType")==0 ){
      
      size_t found = CurStr.find_first_of("#");
      if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
      found = CurStr.find_first_of(" ");
      
      if(found==string::npos){
	oerr <<"\n\nERROR: There is no any space in [LoggingType] string" << endl;
	return LoggingType;
      }else{

	LoggingType = CurStr.substr(found+1);
	while(LoggingType.c_str()[0]==' '){ LoggingType = LoggingType.substr(1); }
	found = LoggingType.find_first_of(" ");
	if(found!=string::npos){   LoggingType = LoggingType.substr(0, found); }
	
      }//end if(space)
    }//end if(LoggingType)
    
    CurStr.clear();
  }//end while()

  
  if (!is.eof()){
    
    oerr <<"\n\nERROR: config file is not read" << endl;
    is.close();
    return LoggingType;
  }//end if(eof)

  is.close();
  return LoggingType;
}


//=================================================================================================================== 'Source
nwlSourceRecord  nwlConfigParser::GetSource(){
  ifstream is = OpenFile();
  
  nwlSourceRecord stcSource;
  string CurStr;

  while(getline (is,CurStr)){
    
    if( CurStr.compare("Source")==0 ){
      
      while(getline (is,CurStr) && CurStr.compare("Material")!=0){
	
	if(CurStr.c_str()[0]=='+'){

	  string Type, Spectrum, Direction, Pencil, Particle, Position;
	  size_t found = CurStr.find_first_of("#");
	  if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
	  found = CurStr.find_first_of(" ");

	  if(found==string::npos){
	    oerr <<"\n\nERROR: There are no enough spaces in [Source] section" << endl;
	    return stcSource;
	  }else{

	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }

	    if(CurStr.compare(0, 4, "Type")==0){

	      found = CurStr.find_first_of(" ");

	        if(found!=string::npos){
		Type = CurStr.substr(found+1);
		while(Type.c_str()[0]==' '){ Type = Type.substr(1); }
		found = Type.find_first_of(" ");
		if(found!=string::npos){  Type = Type.substr(0, found); }

		stcSource.Type = Type;
	      }//end !npos

	    }else if(CurStr.compare(0, 8, "Spectrum")==0){

	      found = CurStr.find_first_of(" ");
	      
	      if(found!=string::npos){
		Spectrum = CurStr.substr(found+1);
		while(Spectrum.c_str()[0]==' '){ Spectrum = Spectrum.substr(1); }

		if(Spectrum.c_str()[0]=='['){

		  Spectrum = Spectrum.substr(1);
		  found = Spectrum.find_first_of("]");
		  if(found!=string::npos){ Spectrum = Spectrum.substr(0, found);}

		   bool ParseSpectrum = true;
		   while(ParseSpectrum){
		     
		     while(Spectrum.c_str()[0]==' '){ Spectrum = Spectrum.substr(1); }
		     
		     string Energy;
		     double E;
		     
		     
		    found = Spectrum.find_first_of(",");
		    if(found!=string::npos){
		      
		      Energy = Spectrum.substr(0, found);
		      Spectrum = Spectrum.substr(found+1);
		      E = atof(Energy.c_str());

		      stcSource.Energy.push_back(E);
		      
		    }else{ParseSpectrum = false;}
		    
		    while(Spectrum.c_str()[0]==' '){ Spectrum = Spectrum.substr(1); }
		    
		    string Intensity;
		    double I;
		    
		    found = Spectrum.find_first_of(",");
		    if(found!=string::npos){
		      
		      Intensity = Spectrum.substr(0, found);
		      Spectrum = Spectrum.substr(found+1);
		      I = atof(Intensity.c_str());

		      stcSource.Intensity.push_back(I);
		      
		    }else{
		      
		      found = Spectrum.find_first_of(" ");
		      if(found!=string::npos){ Spectrum = Spectrum.substr(0, found); }
		      
		      I = atof(Spectrum.c_str());

		      stcSource.Intensity.push_back(I);
		      
		      ParseSpectrum = false;
		    }//end else
		    
		   }//end while(ParseSpectrum)


		   if(stcSource.Energy.size() != stcSource.Intensity.size()) {   oerr <<"\n\nERROR: Different number of energy and intensity in [Source.Spectrum]" << endl;   return stcSource; }
		}//end if([)
			
	      }//end !npos
	      
	    }else if(CurStr.compare(0, 9, "Direction")==0){

	      found = CurStr.find_first_of(" ");
	      
	      if(found!=string::npos){

		Direction = CurStr.substr(found+1);
		while(Direction.c_str()[0]==' '){ Direction = Direction.substr(1); }

		if(Direction.compare(0, 3, "4pi")==0){

		  found = Direction.find_first_of(" ");
		  if(found!=string::npos){ Direction = Direction.substr(0, found); }

		  stcSource.Direction = Direction;

		}else if(Direction.compare(0, 6, "Pencil")==0){

		  found = Direction.find_first_of(" ");
		  if(found!=string::npos){

		    Pencil = Direction.substr(found+1);
		    while(Pencil.c_str()[0]==' '){ Pencil = Pencil.substr(1); }
		    
		    Direction = Direction.substr(0, found);
		  }//end if(' ')
		  
		  stcSource.Direction = Direction;

		  if(Pencil.c_str()[0]=='('){

		    Pencil = Pencil.substr(1);
		    found = Pencil.find_first_of(")");
		    if(found!=string::npos){ Pencil = Pencil.substr(0, found);}

		     bool ParsePencil = true;
		     while(ParsePencil){

		       while(Pencil.c_str()[0]==' '){ Pencil = Pencil.substr(1); }
		       
		       string XYZ;
		       double Xi;
		       
		       found = Pencil.find_first_of(",");
		       if(found!=string::npos){
			 
			 XYZ = Pencil.substr(0, found);
			 Pencil = Pencil.substr(found+1);
			 Xi = atof(XYZ.c_str());
			 
			 stcSource.Pencil.push_back(Xi);
			 
		       }else{
			 
			 found = Pencil.find_first_of(" ");
			 if(found!=string::npos){ Pencil = Pencil.substr(0, found); }
			 
			 Xi = atof(Pencil.c_str());
			 
			 stcSource.Pencil.push_back(Xi);
			 
			 ParsePencil = false;
		       }//end else
		       
		     }//end while(ParsePencil)

		     if(stcSource.Pencil.size() != 3){oerr <<"\n\nERROR: Not enough parameters in [Source.Pencil]" << endl;   return stcSource; }
		     
		  }//end if("(")
		  
		}//end else if(Direction)
		
	      }//end !npos
	      
	    }else if(CurStr.compare(0, 8, "Particle")==0){

	      found = CurStr.find_first_of(" ");
	      
	      if(found!=string::npos){
		Particle = CurStr.substr(found+1);
		while(Particle.c_str()[0]==' '){ Particle = Particle.substr(1); }
		found = Particle.find_first_of(" ");
		if(found!=string::npos){  Particle = Particle.substr(0, found); }
		
		stcSource.Particle = Particle;
	      }//end !npos
	      
	      
	    }else if(CurStr.compare(0, 8, "Position")==0){

	      found = CurStr.find_first_of(" ");
	      if(found!=string::npos){
		
		Position = CurStr.substr(found+1);
		while(Position.c_str()[0]==' '){ Position = Position.substr(1); }
		
	      }//end if(' ')

	     
	      if(Position.c_str()[0]=='('){
		
		Position = Position.substr(1);
		found = Position.find_first_of(")");
		if(found!=string::npos){ Position = Position.substr(0, found);}
		
		bool ParsePosition = true;
		while(ParsePosition){
		  
		  while(Position.c_str()[0]==' '){ Position = Position.substr(1); }
		  
		  string XYZ;
		  double Xi;
		  
		  found = Position.find_first_of(",");
		  if(found!=string::npos){
		    
		    XYZ = Position.substr(0, found);
		    Position = Position.substr(found+1);
		    Xi = atof(XYZ.c_str());
		    
		    stcSource.Position.push_back(Xi);
		    
		  }else{
		    
		    found = Position.find_first_of(" ");
		    if(found!=string::npos){ Position = Position.substr(0, found); }
		    
		    Xi = atof(Position.c_str());
		    
		    stcSource.Position.push_back(Xi);
		    
		    ParsePosition = false;
		  }//end else
		  
		}//end while(ParsePosition)

		if(stcSource.Position.size() != 3){oerr <<"\n\nERROR: Not enough parameters in [Source.Position]" << endl;   return stcSource; }
		
	      }//end if("(")
	      
	      
	    }//end else if	    
	  }//end else	  
	}//end if(+)
	
	CurStr.clear();
      }//end while(!=Material)
    }//end if(Source)
    
    CurStr.clear();
  }//end while()
  
  
  
  if (!is.eof()){
    
    oerr <<"\n\nERROR: config file is not read" << endl;
    is.close();
    return stcSource;
  }//end if(eof)
  
  
  is.close();
  return stcSource;
}


//=================================================================================================================== Material
bool nwlConfigParser::GetMaterial(vector<nwlMaterialRecord>& Materials){
  ifstream is = OpenFile();

  string CurStr;

  while(getline (is,CurStr)){

       if( CurStr.compare("Material")==0 ){
      
	 while(getline (is,CurStr) && CurStr.compare("Geometry")!=0){

	   
	   if( (CurStr.c_str()[0]=='+') && (CurStr.c_str()[1]!='+') ){

	     string Name;
	     double Density;
	     nwlMaterialRecord Material;
	     size_t found = CurStr.find_first_of("#");
	     if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
	     found = CurStr.find_first_of(" ");	     
	     
	     if(found==string::npos){
	       oerr <<"\n\nERROR: There are no enough spaces in [Material] section" << endl;
	       return false;
	     }else{
	       
	       CurStr = CurStr.substr(found+1);
	       while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	       found = CurStr.find_first_of(" ");
	       if(found!=string::npos){ CurStr = CurStr.substr(0, found); }

	       Material.Name = CurStr;
	       
	     }//end else(npos)

	     
	     
	     while(getline (is,CurStr) && (CurStr.c_str()[0]=='+') && (CurStr.c_str()[1]=='+') ){

	       CurStr = CurStr.substr(2);
	       found = CurStr.find_first_of("#");
	       if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
	       found = CurStr.find_first_of(" ");

	       if(found==string::npos){
		 oerr <<"\n\nERROR: There are no enough spaces in ["<< Material.Name <<"]" << endl;
		 return false;
	       }else{
		 
		 CurStr = CurStr.substr(found+1);
		 while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }

		 string Element;
		 found = CurStr.find_first_of(" ");

		 if(found!=string::npos){

		   Element = CurStr.substr(0, found);
		   CurStr = CurStr.substr(found+1);
		   while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
		   found = CurStr.find_first_of(" ");
		   if(found!=string::npos){ CurStr = CurStr.substr(0, found); }


		 
		   double Fraction = atof(CurStr.c_str());

		   if(Element.compare("Density")==0){

		      Material.Density = Fraction;
		      
		   }else{

		     Material.Element.push_back(Element);
		     Material.Fraction.push_back(Fraction);

		   }//end else(Element)
		   
		 }// end if(!npos)

		 
		 
	       }//end else(npos)
	       
	       CurStr.clear();
	     }//end while(++)

	     Materials.push_back(Material);
	   }//end if(+)
	   
	   CurStr.clear();
	 }//end while(!=Geometry)
       }//end if(Material)
    
    CurStr.clear();
  }//end while()

  
   if (!is.eof()){
    
     oerr <<"\n\nERROR: config file is not read" << endl;
    is.close();
    return false;
  }//end if(eof)
  
  
  is.close();
  return true;
}

//================================================================================================================ Geometry
bool nwlConfigParser::GetGeometry(vector<nwlGeometryRecord>& Volumes){
  ifstream is = OpenFile();

  string CurStr;

  string PreVolume;
  size_t PreLevel, CurLevel;
  vector<string> tmpVolume;
  vector<size_t> tmpLevel;
  
  while(getline (is,CurStr)){

    if( CurStr.compare("Geometry")==0 ){

      while(getline (is,CurStr) && CurStr.compare("Detector")!=0){

	if(CurStr.c_str()[0]=='+'){

	  string Name, exVolume, Material, Type, Parameters, Position, Rotation;
	  nwlGeometryRecord Volume;
	  size_t found = CurStr.find_first_of("#");
	  if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
	  
	  if(CurStr.c_str()[1]!='+'){

	    PreLevel = CurStr.find_last_of("+");
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Geometry] section" << endl;
	      return false;
	    }
	    
	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Geometry] section" << endl;
	      return false;
	    }
	    
	    PreVolume = CurStr.substr(0, found);
	    CurStr = CurStr.substr(found+1);

	    Volume.Name = PreVolume;

	    
	  

	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in ["<< Volume.Name <<"]" << endl;
	      return false;
	    }

	    
	    Material = CurStr.substr(0, found);
	    CurStr = CurStr.substr(found+1);
	    
	    Volume.Material = Material;

	    
	    
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in ["<< Volume.Name <<"]" << endl;
	      return false;
	    }


	    Type = CurStr.substr(0, found);
	    CurStr = CurStr.substr(found+1);
	    
	    Volume.Type = Type;


	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of("(");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There is no [(] in ["<< Volume.Name <<"]" << endl;
	      return false;
	    }

	    Parameters = CurStr.substr(0, found+1);
	    CurStr = CurStr.substr(found+1);

	    while(Parameters.c_str()[0]!='('){

	      while(Parameters.c_str()[0]==' '){ Parameters = Parameters.substr(1); }

	      found = Parameters.find_first_of(" ");

	      if(found!=string::npos){
	      string Parameter = Parameters.substr(0, found);
	      Parameters = Parameters.substr(found+1);

	      double par = atof(Parameter.c_str());

	      Volume.Parameters.push_back(par);
	      }//end if(!npos)

	    }//end while(!="(")


	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(")");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There is no [)] in ["<< Volume.Name <<"]" << endl;
	      return false;
	    }


	    Position =  CurStr.substr(0, found+1);
	    CurStr = CurStr.substr(found+1);

	    while(Position.c_str()[0]!=')'){

	      while(Position.c_str()[0]==' '){ Position = Position.substr(1); }
	      
	      found = Position.find_first_of(",");

	      if(found!=string::npos){
		string XYZ = Position.substr(0, found);
		Position = Position.substr(found+1);
		
		double Xi = atof(XYZ.c_str());
		
		Volume.Position.push_back(Xi);
	      }else{
		
		found = Position.find_first_of(")");

		string XYZ = Position.substr(0, found);
		Position = Position.substr(found);

		double Xi = atof(XYZ.c_str());
		
		Volume.Position.push_back(Xi);
		
	      }//end if(!npos)
	      
	    }//end while(!=")")

	    if( (Volume.Position.size() != 0) && (Volume.Position.size() != 3) ){oerr <<"\n\nERROR: Not enough [Position] parameters in Volume ["<< Volume.Name <<"]" << endl;   return false; }

	    
	     while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	     if(CurStr.c_str()[0]=='['){ CurStr = CurStr.substr(1); }
	     else{ oerr <<"\n\nERROR: There is no [[] in ["<< Volume.Name <<"]" << endl;  return false; }

	     found = CurStr.find_first_of("]");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There is no []] in ["<< Volume.Name <<"]" << endl;
	      return false;
	    }


	    Rotation =  CurStr.substr(0, found+1);
	    CurStr = CurStr.substr(found+1);

	    while(Rotation.c_str()[0]!=']'){

	      while(Rotation.c_str()[0]==' '){ Rotation = Rotation.substr(1); }
	      
	      found = Rotation.find_first_of(",");

	       if(found!=string::npos){
		 string XYZ = Rotation.substr(0, found);
		 Rotation = Rotation.substr(found+1);
		 
		 double Xi = atof(XYZ.c_str());
		 
		 Volume.Rotation.push_back(Xi);
	       }else{
		 
		 found = Rotation.find_first_of("]");
		 
		 string XYZ = Rotation.substr(0, found);
		 Rotation = Rotation.substr(found);
		 
		 double Xi = atof(XYZ.c_str());
		 
		 Volume.Rotation.push_back(Xi);
		 
	       }//end if(!npos)
	      
	    }//end while(!="]")


	    if( (Volume.Rotation.size() != 0) && (Volume.Rotation.size() != 9) ){ oerr <<"\n\nERROR: Not enough [Rotation matrix] parameters in Volume ["<< Volume.Name <<"]" << endl;   return false; }

	       
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    if(CurStr.c_str()[0] == 0){ Volume.Weight = 1.0; }
	    else{

	      found = CurStr.find_first_of(" ");
	      if(found!=string::npos){ CurStr = CurStr.substr(0, found); }

	      Volume.Weight = atof(CurStr.c_str());
	      
	      
	    }//end else

	     
	  

	    
	    
	  }else{

	    CurLevel = CurStr.find_last_of("+");
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Geometry] section" << endl;
	      return false;
	    }
	    
	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Geometry] section" << endl;
	      return false;
	    }
	    
	    Name = CurStr.substr(0, found);
	    CurStr = CurStr.substr(found+1);
	    
	    Volume.Name = Name;


	    
	    //----------------------------------------------------------------- Volume nesting
	    if( CurLevel > PreLevel){

	      Volume.exVolume = PreVolume;

	      tmpVolume.push_back(PreVolume);
	      tmpLevel.push_back(CurLevel);

	      PreVolume = Name;
	      PreLevel = CurLevel;

	    } else if(CurLevel == PreLevel){

	      int iV;
	      for(unsigned int i = 0; i < tmpLevel.size(); i++) { if(CurLevel == tmpLevel[i])  iV = i; }

	      Volume.exVolume = tmpVolume[iV];

	      PreVolume = Name;
	     

	    }else{

	      int iV;
	      for(unsigned int i = 0; i < tmpLevel.size(); i++) { if(CurLevel == tmpLevel[i])  iV = i; }

	      Volume.exVolume = tmpVolume[iV];

	      PreVolume = Name;
	      PreLevel = CurLevel;

	    }//if(CurLevel)
	    //===================================================================================

	    

	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in Volume ["<< Volume.Name <<"]" << endl;
	      return false;
	    }
	    
	    
	    Material = CurStr.substr(0, found);
	    CurStr = CurStr.substr(found+1);
	    
	    Volume.Material = Material;
	    
	    
	    
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in Volume ["<< Volume.Name <<"]" << endl;
	      return false;
	    }


	    Type = CurStr.substr(0, found);
	    CurStr = CurStr.substr(found+1);
	    
	    Volume.Type = Type;


	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of("(");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There is no [(] in Volume ["<< Volume.Name <<"]" << endl;
	      return false;
	    }

	    Parameters = CurStr.substr(0, found+1);
	    CurStr = CurStr.substr(found+1);

	    while(Parameters.c_str()[0]!='('){

	      while(Parameters.c_str()[0]==' '){ Parameters = Parameters.substr(1); }

	      found = Parameters.find_first_of(" ");

	      if(found!=string::npos){
	      string Parameter = Parameters.substr(0, found);
	      Parameters = Parameters.substr(found+1);

	      double par = atof(Parameter.c_str());

	      Volume.Parameters.push_back(par);
	      }//end if(!npos)

	    }//end while(!="(")


	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(")");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There is no [)] in Volume ["<< Volume.Name <<"]" << endl;
	      return false;
	    }


	    Position =  CurStr.substr(0, found+1);
	    CurStr = CurStr.substr(found+1);

	    while(Position.c_str()[0]!=')'){

	      while(Position.c_str()[0]==' '){ Position = Position.substr(1); }
	      
	      found = Position.find_first_of(",");

	      if(found!=string::npos){
		string XYZ = Position.substr(0, found);
		Position = Position.substr(found+1);
		
		double Xi = atof(XYZ.c_str());
		
		Volume.Position.push_back(Xi);
	      }else{
		
		found = Position.find_first_of(")");

		string XYZ = Position.substr(0, found);
		Position = Position.substr(found);

		double Xi = atof(XYZ.c_str());
		
		Volume.Position.push_back(Xi);
		
	      }//end if(!npos)
	      
	    }//end while(!=")")


	    if( (Volume.Position.size() != 0) && (Volume.Position.size() != 3) ){oerr <<"\n\nERROR: Not enough [Position] parameters in Volume ["<< Volume.Name <<"]" << endl;   return false; }

	    
	     while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	     if(CurStr.c_str()[0]=='['){ CurStr = CurStr.substr(1); }
	     else{ oerr <<"\n\nERROR: There is no [[] in Volume ["<< Volume.Name <<"]" << endl;  return false; }

	     found = CurStr.find_first_of("]");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There is no []] in Volume ["<< Volume.Name <<"]" << endl;
	      return false;
	    }


	    Rotation =  CurStr.substr(0, found+1);
	    CurStr = CurStr.substr(found+1);

	    while(Rotation.c_str()[0]!=']'){

	      while(Rotation.c_str()[0]==' '){ Rotation = Rotation.substr(1); }
	      
	      found = Rotation.find_first_of(",");

	       if(found!=string::npos){
		 string XYZ = Rotation.substr(0, found);
		 Rotation = Rotation.substr(found+1);
		 
		 double Xi = atof(XYZ.c_str());
		 
		 Volume.Rotation.push_back(Xi);
	       }else{
		 
		 found = Rotation.find_first_of("]");
		 
		 string XYZ = Rotation.substr(0, found);
		 Rotation = Rotation.substr(found);
		 
		 double Xi = atof(XYZ.c_str());
		 
		 Volume.Rotation.push_back(Xi);
		 
	       }//end if(!npos)
	      
	    }//end while(!="]")

	    if( (Volume.Rotation.size() != 0) && (Volume.Rotation.size() != 9) ){ oerr <<"\n\nERROR: Not enough [Rotation matrix] parameters in Volume ["<< Volume.Name <<"]" << endl;   return false; }

	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    if(CurStr.c_str()[0] == 0){ Volume.Weight = 1.0; }
	    else{

	      found = CurStr.find_first_of(" ");
	      if(found!=string::npos){ CurStr = CurStr.substr(0, found); }

	      Volume.Weight = atof(CurStr.c_str());
	      
	      
	    }//end else



	  }//end else(+ or ++...)

	   Volumes.push_back(Volume);

	   if( (Volume.Type.compare("Box")==0) && (Volume.Parameters.size() != 3) ){oerr <<"\n\nERROR: Wrong number of [Box] parameters (not 3) in Volume ["<< Volume.Name <<"]" << endl;   return false;  }
	   else if( (Volume.Type.compare("Tub")==0) && (Volume.Parameters.size() != 5) ){oerr <<"\n\nERROR: Wrong number of [Tub] parameters (not 5) in Volume ["<< Volume.Name <<"]" << endl;   return false;}
	   else if( (Volume.Type.compare("Con")==0) && (Volume.Parameters.size() != 7) ){oerr <<"\n\nERROR: Wrong number of [Con] parameters (not 7) in Volume ["<< Volume.Name <<"]" << endl;   return false;}
	   else if( (Volume.Type.compare("Trd")==0) && (Volume.Parameters.size() != 5) ){oerr <<"\n\nERROR: Wrong number of [Trd] parameters (not 5) in Volume ["<< Volume.Name <<"]" << endl;   return false;}
	   else if( (Volume.Type.compare("Sphere")==0) && (Volume.Parameters.size() != 6) ){oerr <<"\n\nERROR: Wrong number of [Sphere] parameters (not 6) in Volume ["<< Volume.Name <<"]" << endl;   return false;}

	   
	}//end if(+)
       
	CurStr.clear();
      }//while(!=Detector)
    }//end if(Geometry)
    
    CurStr.clear();
  }//end while()
  


  
  if (!is.eof()){
    
    oerr <<"\n\nERROR: config file is not read" << endl;
    is.close();
    return false;
  }//end if(eof)
  
  
  is.close();
  return true;
}


//================================================================================================================ Detector
bool nwlConfigParser::GetDetector(vector<string>& Detectors){
  ifstream is = OpenFile();
  
  string CurStr;
  
  while(getline (is,CurStr)){

    if( CurStr.compare("Detector")==0 ){

      while(getline (is,CurStr) && CurStr.compare("Output")!=0){
	
	if(CurStr.c_str()[0]=='+'){

	  size_t found = CurStr.find_first_of("#");
	  if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
	  found = CurStr.find_first_of(" ");
	  
	  if(found==string::npos){
	    oerr <<"\n\nERROR: There are no enough spaces in [Detector] section" << endl;
	    return false;
	  }else{
	    
	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    if(found!=string::npos){ CurStr = CurStr.substr(0, found); }

	    Detectors.push_back(CurStr);
	  }//end else
	  
	}//end if(+)
	CurStr.clear();
      }//while()
    }//end if(Detector)
    
    CurStr.clear();
  }//end while()
  
  
  if (!is.eof()){
    
    oerr <<"\n\nERROR: config file is not read" << endl;
    is.close();
    return false;
  }//end if(eof)
  
  
  is.close();
  return true;
}

//=========================================================================================================== GetOutput
bool nwlConfigParser::GetOutput(vector<nwlH1Record>& H1Ds, vector<nwlH2Record>& H2Ds, bool WriteNtuple){
  ifstream is = OpenFile();

  string CurStr;
  
  while(getline (is,CurStr)){
    
    if( CurStr.compare("Output")==0 ){
      
      while(getline (is,CurStr)){
	
	if(CurStr.c_str()[0]=='+'){
	  
	  size_t found = CurStr.find_first_of("#");
	  if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
	  found = CurStr.find_first_of(" ");

	  if(found==string::npos){
	    oerr <<"\n\nERROR: There are no enough spaces in [Output] section" << endl;
	    return false;
	  }//end if(" ")
	  
	  
	  CurStr = CurStr.substr(found+1);
	  while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	  found = CurStr.find_first_of(" ");

	  if(found==string::npos){
	    oerr <<"\n\nERROR: There are no enough spaces in [Output] section" << endl;
	    return false;
	  }//end if(" ")
	  

	  string Type;
	  Type = CurStr.substr(0, found);
	  CurStr = CurStr.substr(found+1);
	  while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	  
	  if(Type.compare("H1D")==0){

	    nwlH1Record H1D;

	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H1D)" << endl;
	      return false;
	    }//end if(" ")

	   
	    H1D.PhysQ  = CurStr.substr(0, found);

	    if( (H1D.PhysQ.compare("Energy")!=0) && (H1D.PhysQ.compare("Time")!=0) && (H1D.PhysQ.compare("X")!=0) &&  (H1D.PhysQ.compare("Y")!=0) && (H1D.PhysQ.compare("Z")!=0) && (H1D.PhysQ.compare("ProcessID")!=0) && (H1D.PhysQ.compare("NucleusA")!=0) && (H1D.PhysQ.compare("NucleusZ")!=0) ){
	      oerr <<"\n\nERROR: " << H1D.PhysQ  << "is invalid value of [physical quantity] in [H1D]" << endl;
	      return false;
	    }
	    
	    
	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H1D)" << endl;
	      return false;
	    }//end if(" ")

	    string Nbins;
	    Nbins = CurStr.substr(0, found);
	    H1D.Nbins = atoi(Nbins.c_str());


	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H1D)" << endl;
	      return false;
	    }//end if(" ")

	    string Xmin;
	    Xmin = CurStr.substr(0, found);
	    H1D.Xmin = atof(Xmin.c_str());

	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
	    
	    H1D.Xmax = atof(CurStr.c_str());
	    
	    H1Ds.push_back(H1D);

	  }else if(Type.compare("H2D")==0){

	    nwlH2Record H2D;

	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H2D)" << endl;
	      return false;
	    }//end if(" ")
	    
	    
	    H2D.PhysQ_x  = CurStr.substr(0, found);

	    if( (H2D.PhysQ_x.compare("Energy")!=0) && (H2D.PhysQ_x.compare("Time")!=0) && (H2D.PhysQ_x.compare("X")!=0) &&  (H2D.PhysQ_x.compare("Y")!=0) && (H2D.PhysQ_x.compare("Z")!=0) && (H2D.PhysQ_x.compare("ProcessID")!=0) && (H2D.PhysQ_x.compare("NucleusA")!=0) && (H2D.PhysQ_x.compare("NucleusZ")!=0) ){
	      oerr <<"\n\nERROR: " << H2D.PhysQ_x  << "is invalid value of [physical quantity x] in [H2D]" << endl;
	      return false;
	    }
	    
	    
	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H2D)" << endl;
	      return false;
	    }//end if(" ")

	    string Nbinsx;
	    Nbinsx = CurStr.substr(0, found);
	    H2D.Nbins_x = atoi(Nbinsx.c_str());

	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H2D)" << endl;
	      return false;
	    }//end if(" ")
	    
	    string Xmin;
	    Xmin = CurStr.substr(0, found);
	    H2D.Xmin = atof(Xmin.c_str());


	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H2D)" << endl;
	      return false;
	    }//end if(" ")
	    
	    string Xmax;
	    Xmax = CurStr.substr(0, found);
	    H2D.Xmax = atof(Xmax.c_str());

	    
	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H2D)" << endl;
	      return false;
	    }//end if(" ")
	    

	     H2D.PhysQ_y  = CurStr.substr(0, found);

	    if( (H2D.PhysQ_y.compare("Energy")!=0) && (H2D.PhysQ_y.compare("Time")!=0) && (H2D.PhysQ_y.compare("X")!=0) &&  (H2D.PhysQ_y.compare("Y")!=0) && (H2D.PhysQ_y.compare("Z")!=0) && (H2D.PhysQ_y.compare("ProcessID")!=0) && (H2D.PhysQ_y.compare("NucleusA")!=0) && (H2D.PhysQ_y.compare("NucleusZ")!=0) ){
	      oerr <<"\n\nERROR: " << H2D.PhysQ_y  << "is invalid value of [physical quantity y] in [H2D]" << endl;
	      return false;
	    }
	    

	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H2D)" << endl;
	      return false;
	    }//end if(" ")
	    
	    string Nbinsy;
	    Nbinsy = CurStr.substr(0, found);
	    H2D.Nbins_y = atoi(Nbinsy.c_str());


	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    
	    if(found==string::npos){
	      oerr <<"\n\nERROR: There are no enough spaces in [Output] section (H2D)" << endl;
	      return false;
	    }//end if(" ")
	    
	    string Ymin;
	    Ymin = CurStr.substr(0, found);
	    H2D.Ymin = atof(Ymin.c_str());


	    CurStr = CurStr.substr(found+1);
	    while(CurStr.c_str()[0]==' '){ CurStr = CurStr.substr(1); }
	    found = CurStr.find_first_of(" ");
	    if(found!=string::npos){ CurStr = CurStr.substr(0, found); }
	    
	    H2D.Ymax = atof(CurStr.c_str());
	    
	    H2Ds.push_back(H2D);
	    
	    
	  }else if(Type.compare("WriteNtuple")==0){

	    found = CurStr.find_first_of(" ");
	    if(found!=string::npos){ CurStr = CurStr.substr(0, found); }

	    if( CurStr.compare("True")==0 ){

	      WriteNtuple = true;

	    }else if(CurStr.compare("False")==0){

	      WriteNtuple = false;
	      
	    }else{
	      oerr <<"\n\nERROR: invalid value of [WriteNtuple] in [Output] section" << endl;
	      return false;
	    }//end else(WriteNtuple)

	  }//end else if(Type)
	  
	  
	  
	}//end if(+)
	
    	CurStr.clear();
      }//while()
    }//end if(Output)
    
    CurStr.clear();
  }//end while()
  
  
  if (!is.eof()){
    
    oerr <<"\n\nERROR: config file is not read" << endl;
    is.close();
    return false;
  }//end if(eof)
  
  
  is.close();
  return true;
}
