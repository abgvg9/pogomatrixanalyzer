//Ari Ginsparg
//NonEMusDingo
//Program to take in Pokemon GO battle matrices to compose optimal teams
//Requires a matrix file in .csv format
//For Tempest Cup, run the following executable: ./a.out <TempestMatrix.csv


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctype.h>
#include <cmath>

using namespace std;

int main()
{
  //2d vector to hold the values of the matrix
  //corresponding indices match indices of the species and moveset vectors
  vector<vector<float>> matrix;

  //1d vectors to hold the species names and movesets
  vector<string> species;
  vector<string> moveset;

  //initial string matrix
  //will use string matipulation to extract data and fill the other vectors
  vector<string> initial_matrix;

  string line_buffer;

  //take in the matrix (piped in via command line)
  while(getline(cin, line_buffer))
  {
    initial_matrix.push_back(line_buffer);
  }

  /*
  //print out the raw contents of the matrix
  for (int i = 0; i < initial_matrix.size(); i++)
  {
    cout << initial_matrix[i] << endl;
  }
  */

  //fill the matrix, species, and moveset vectora
  //skip indices 0 and 1 of initial_matrix due to having irrelevant information
  for(int i = 2; i < initial_matrix.size(); i++)
  {
    //denote the number of commas encountered
    //species comes before the first comma (0), moveset before the second (1), and matrix values afterwards (2+)
    int comma_counter = 0;
    string substring = "";

    //vector to hold a line that will be fed into the matrix
    vector<float> matrix_line;

    //run through the line of initial_matrix to extract all values on the line
    for(int j = 0; j < initial_matrix[i].size(); j++)
    {
      //take the character in question
      char character = initial_matrix[i][j];
      //cout << character << j << " " << initial_matrix[i].size() << endl;

      //change behavior, based on whether character is a comma or not
      if(character == ',')
      {
        if(comma_counter == 0) //species
        {
          species.push_back(substring);
        }
        else if(comma_counter == 1) //moveset
        {
          moveset.push_back(substring);
        }
        else //matrix value
        {
          //convert string to float
          float temp = atof(substring.c_str());
          matrix_line.push_back(temp);
        }

        substring = "";
        comma_counter++;
      }
      else
      {
        substring += initial_matrix[i][j];
      }
    }

    //get the last value of the matrix line
    float temp = atof(substring.c_str());
    matrix_line.push_back(temp);
    //add matrix_line to the matrix
    matrix.push_back(matrix_line);

  }

  //print out the constructed vectors
  for(int i = 0; i < species.size(); i++)
  {
    cout << species[i] << " ";
    cout << moveset[i];
    for(int j = 0; j < species.size(); j++)
    {
      cout << " " << matrix[i][j];
    }
    cout << endl;
  }

}
