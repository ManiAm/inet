/*******************************************************************
*
*    This library is free software, you can redistribute it 
*    and/or modify 
*    it under  the terms of the GNU Lesser General Public License 
*    as published by the Free Software Foundation; 
*    either version 2 of the License, or any later version.
*    The library is distributed in the hope that it will be useful, 
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*    See the GNU Lesser General Public License for more details.
*
*
*********************************************************************/
#include <iostream>
#include "LIBtable.h"



Define_Module( LIBTable );


void LIBTable::initialize()
{
// const char *libFilename = par("LibTableFileName").stringValue(); -- FIXME was not used (?)
// const char *prtFilename = par("PrtTableFileName").stringValue(); -- FIXME was not used (?)

 

      // Read routing table file

        // Abort simulation if no routing file could be read
   /*
       if (readLibTableFromFile(libFilename) == -1)
          ev << "No input lib table file "  <<  libFilename;


       if (readPrtTableFromFile(prtFilename) == -1)
          ev << "No input prt table file " << libFilename;

          printTables();              */

}


void LIBTable::handleMessage(cMessage *msg)
{

// Nothing

}



int LIBTable::readLibTableFromFile(const char *filename)
{

  ifstream fin(filename);    // create a "pointer" to the file
  if (!fin)                  // if the file does not exist
   return -1;

  char line[100];

  string::size_type loc;//unsigned int loc=0;
  StringTokenizer *tokenizer;

//Move to the first record

 while (fin.getline(line,100) && !fin.eof())
 {

       string *s=new string(line);
       loc = s->find( ConstType::libDataMarker, 0 );

       if( loc != string::npos )
       {
            fin.getline(line,100); //Get into the blank line
            break;
        }    

  }



  while (fin.getline(line,100) && !fin.eof())
  {

     lib_type record;

     string *aField;

     string sline(line);

     string deliminator(" ,    ");

      //Reach the end of table 

     if(sline.size()==0) //If string(sline).empty())
         break;

      tokenizer =new StringTokenizer(sline, deliminator);

      //Get the first field - Incoming Label
      if ((aField=(tokenizer->nextToken()))!=NULL)
         record.inLabel= atoi(aField->c_str());
      else
        record.inLabel= -1;
      
      //Get the second field - Incoming Interface
      if ((aField=(tokenizer->nextToken()))!=NULL)
        record.inInterface= *aField;
      else 
          record.inInterface=string(ConstType::UnknownData);

      //Get the third field - Outgoing Label
      if ((aField=(tokenizer->nextToken()))!=NULL)
        record.outLabel=atoi(aField->c_str());
      else
        record.outLabel= -1;
      
      //Get the fourth field - Outgoing Interface
      if ((aField=(tokenizer->nextToken()))!=NULL)
        record.outInterface=*aField;
      else
          record.outInterface= string(ConstType::UnknownData);

      //Get the fifth field - Optcode
      if ((aField=(tokenizer->nextToken()))!=NULL)
         record.optcode= atoi(aField->c_str());
      else
        record.optcode= -1;

    //Insert into the vector
      lib.push_back(record);

  }

  return 0; 

}



int LIBTable::readPrtTableFromFile(const char* filename)
{

    ifstream fin(filename);    // create a "pointer" to the file
    if (!fin)                  // if the file does not exist
        return -1;

    char line[100];

    string::size_type loc;//unsigned int loc=0;
    StringTokenizer *tokenizer;

    //Move to the first record
    while (fin.getline(line,100) && !fin.eof())
    {
        string *s=new string(line);
        loc = s->find( ConstType::prtDataMarker, 0 );
        
        if( loc != string::npos )
        {
            fin.getline(line,100); //Get into the blank line
            break;
        }
    }
    
    while (fin.getline(line,100) && !fin.eof())
    {   

        prt_type record;
        
        //A field of the table record
        string *aField;
        
        //Variable to hold the a record in string form
        string sline(line);
        string deliminator(" ,    ");

        if (sline.size()==0)
            break; //Reach the end of table

        tokenizer =new StringTokenizer(sline, deliminator);
        if ((aField=(tokenizer->nextToken()))!=NULL)
          {

            //char *cPrefix =new char[aField->length()+1];

            //aField->copy(cPrefix,string::npos);
            //Create new prefix,

            IPAddressPrefix *prefixIPAddress =new IPAddressPrefix((aField->c_str()),ConstType::prefixLength);
            record.fecValue=*prefixIPAddress;

          }
        else
          {          //New empty prefix,
            IPAddressPrefix *prefixIPAddress =new IPAddressPrefix("0.0.0.0",ConstType::prefixLength);
            record.fecValue=*prefixIPAddress;

          }

        if ((aField=(tokenizer->nextToken()))!=NULL)
        {
           //char *cstring =new char[aField->length()+1];

            //aField->copy(cstring,string::npos);

           int x=atoi(aField->c_str());
           record.pos=x;
           }

        else
            record.pos=-1;



      //Add the record 
      prt.push_back(record);
      }
      return 0;

}



void LIBTable::printTables()
{
   int i;
     //Print out the LIB table
     ev << "************LIB TABLE CONTENTS***************** \n";
     ev <<" InL       InInf        OutL     Outf    Optcode  \n";

     for(i=0;i< lib.size();i++)
     ev << lib[i].inLabel << "    "<< lib[i].inInterface.c_str() << " " <<  lib[i].outLabel << "     " << lib[i].outInterface.c_str() << "   " << lib[i].optcode << "\n";


     //Print out the PRT table 
     ev << "*****************PRT TABLE CONTENT**************\n" ;
     ev << "Pos  Fec \n";
        
     for(i=0;i<prt.size();i++)
     ev << prt[i].pos << "    " << prt[i].fecValue <<"\n";

}



int LIBTable::installNewLabel(int outLabel, string inInterface, 
                                        string outInterface, int fec, int optcode)
{

    lib_type *newLabelEntry = new lib_type;
    newLabelEntry->inInterface =inInterface;
    newLabelEntry->outInterface=outInterface;
    newLabelEntry->optcode = optcode;


    //Auto generate inLabel
    newLabelEntry->inLabel =lib.size();
    newLabelEntry->outLabel =outLabel;
    lib.push_back(*newLabelEntry);

    prt_type *aPrt =new prt_type;
    aPrt->pos=(lib.size()-1);
    (aPrt->fecValue)= fec;
    prt.push_back(*aPrt);
    printTables();

    return (newLabelEntry->inLabel);

}

int LIBTable::requestLabelforFec(int fec)
{
    //search in the PRT for exact match of the FEC value
    for(int i=0;i<prt.size();i++)
    {
        if(prt[i].fecValue.getInt()==fec)
        {
            int p = prt[i].pos;

            //Return the outgoing label
            return lib[p].outLabel;
        }
    }
    return -2; 

}



int  LIBTable::findFec(int label, string inInterface)
{
    int pos;
    
    //Search LIB for matching of the incoming interface and label
    for(int i=0;i<lib.size();i++)
    {
        if((lib[i].inInterface == inInterface)&&(lib[i].inLabel==label))
        {
            //Get the position of this match
            pos=i;
            break;
        }

     }

    //Get the FEC value in PRT tabel
     for(int k=0;k<prt.size();k++)
     {
        if(prt[k].pos == pos )
            return prt[k].fecValue.getInt();

     }

    return 0;

}


string LIBTable::requestOutgoingInterface(int fec)
{
    int pos=-1;    

    //Search in PRT for matching of FEC
    for(int k=0;k<prt.size();k++)
    {
        if(prt[k].fecValue.getInt() == fec)
        {
            pos = prt[k].pos;
            break;
        }
    }
    
    if(pos!=-1)
        return lib[pos].outInterface;
    else
        return string("X");
}


string LIBTable::requestOutgoingInterface(string senderInterface, int newLabel)
{

    for(int i=0;i< lib.size();i++)
    {
        if(senderInterface.compare("X") !=0) 
        {
            if((lib[i].inInterface==senderInterface) &&  (lib[i].outLabel==newLabel))
                return lib[i].outInterface;
        }
        else
        if(lib[i].outLabel==newLabel) //LIB of the IR
                return lib[i].outInterface;
    }

    return string("X");
}



string LIBTable::requestOutgoingInterface(string senderInterface, int newLabel, int oldLabel)
{

    if(newLabel !=-1)
       return requestOutgoingInterface(senderInterface, newLabel);

    for(int i=0;i< lib.size();i++)
    {
        if(senderInterface.compare("X") !=0)
        {

            if((lib[i].inInterface==senderInterface) &&  (lib[i].inLabel==oldLabel) )
                return lib[i].outInterface;
        }
        else
        if(lib[i].outLabel==newLabel)
            return lib[i].outInterface;
    }

    return string("X");

}



int LIBTable::requestNewLabel(string senderInterface, int oldLabel)
{

    for(int i=0;i< lib.size();i++)
     if((lib[i].inInterface==senderInterface) &&  (lib[i].inLabel==oldLabel))
         return lib[i].outLabel;

    return -2; //Fail to allocate new label

}

int LIBTable::getOptCode(string senderInterface, int oldLabel)
{
    
    for(int i=0;i< lib.size();i++)
     if((lib[i].inInterface==senderInterface) &&  (lib[i].inLabel==oldLabel))
         return lib[i].optcode;

    return -1; 

}

/*

string LIBTable::ini_requestLabelforDest(IPAddressPrefix *dest)

{

  string label(ConstType::UnknownData);



        for(int i=0;i<prt.size();i++)

        {

                if(prt[i].fecValue.compareTo(dest, ConstType::prefixLength)) //Mean that the dest find its prefix entry

                {

                        int p = prt[i].pos;

                        label =lib[p].outLabel;

                        break;



                }

    

        }

        return label; 



}



void LIBTable::updateTable(label_mapping_type *newMapping)
{

        prt_type *aPrt;

        IPAddressPrefix aFec=newMapping->fec;

        string label =newMapping->label;

    //Update the cross-entry

    for(int i=0;i<lib.size();i++)

    {

        if(lib[i].outLabel.compare(label)==0)

        {

            aPrt->pos=i;

            (aPrt->fecValue)= aFec;

            prt.push_back(*aPrt);

            break;



        }

    }



    return;



}





void LIBTable::updateTable( string inInterface, string outLabel,

                           string outInterface, IPAddressPrefix fec)

{



    string inLabel = string(label).append(string(lib.size()));

    lib_type *newLabelEntry = new lib_type;

    newLabelEntry->inInterface =inInterface;

    newLabelEntry->outInterface=outInterface;

    newLabelEntry->inLabel =inLabel;

    newLabelEntry->outLabel =outLabel;

    lib.push_back(*newLabelEntry);



    prt_type *aPrt;

    aPrt->pos=(lib.size()-1);

    (aPrt->fecValue)= fec;

    prt.push_back(*aPrt);

    

}









string LIBTable::requestNewLabel(string senderInterface, string oldLabel)

{

 for(int i=0;i< lib.size();i++)

     if((lib[i].inInterface==senderInterface) &&  (lib[i].inLabel==oldLabel))

         return lib[i].outLabel;



return string(ConstType::UnknownData);

}



string LIBTable::requestOutgoingInterface(string senderInterface, string newLabel)
{

    for(int i=0;i< lib.size();i++)

    {

        if(senderInterface.compare("OUTSIDE") !=0)

        {

            if((lib[i].inInterface==senderInterface) &&  (lib[i].outLabel==newLabel))

                return lib[i].outInterface;

        }

        else

        if(lib[i].outLabel==newLabel)

                return lib[i].outInterface;

    }



    return string(ConstType::UnknownData);

}



string LIBTable::requestIncommingInterface(IPAddressPrefix fec)

{



        string incommingInterface(ConstType::UnknownData);



        for(int i=0;i<prt.size();i++)

        {

                if(prt[i].fecValue.isEqualTo(fec))

                {

                        int p = prt[i].pos;

                        incommingInterface =lib[p].inInterface;

                        break;



                }
    

        }

        return incommingInterface; 
}



*/





