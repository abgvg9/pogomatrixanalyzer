//Ari Ginsparg
//NonEMusDingo
//Program to take in Pokemon GO battle matrices to compose optimal teams
//Requires a matrix file in .csv format
/*
Run Instructions:
  For compilation:
  g++ team_analyzer_weaknesses.cpp -std=c++11 -o team_analyzer_weaknesses
  To run:
  ./team_analyzer_weaknesses <(input matrix).csv >(output file).csv
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctype.h>
#include <cmath>

using namespace std;

class team
{
  public:
    float score = 0;
    string species1 = "";
    string moveset1 = "";
    string species2 = "";
    string moveset2 = "";
    string species3 = "";
    string moveset3 = "";
    string species4 = "";
    string moveset4 = "";
    string species5 = "";
    string moveset5 = "";
    string species6 = "";
    string moveset6 = "";
};

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

  /*
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
  */

  //a vector to hold all team scores
  vector<team> team_scores;

  //3 nested loops to run through and get all team combinations and associate a score with them
  //the higher the score, the better the team coverage
  for(int i = 0; i < species.size(); i++)
  {
    for(int j = i + 1; j < species.size(); j++)
    {
      //account for avoiding repeat species
      if(species[i] == species[j])
      {
        j++;
      }
      for(int k = j + 1; k < species.size(); k++)
      {
        if(species[j] == species[k])
        {
          k++;
        }
        for(int m = k + 1; m < species.size(); m++)
        {
          if(species[k] == species[m])
          {
            m++;
          }
          for(int n = m + 1; n < species.size(); n++)
          {
            if(species[m] == species[n])
            {
              n++;
            }
            for(int o = n + 1; o < species.size(); o++)
            {
              if(species[n] == species[o])
              {
                o++;
              }

              //cout << "i" << i << ", j" << j << ", k" << k << ", m" << m << ", n" << n << endl;

              //vector to hold the highest scores for all matrix matchups of the 3 species in question
              vector<float> average_scores;

              for(int l = 0; l < species.size(); l++)
              {
                //get the average score across the 3 species being tested
                float average = (matrix[i][l] + matrix[j][l] + matrix[k][l] + matrix[m][l] + matrix[n][l] + matrix[o][l]) / 6;

                //add the high score to the vector
                average_scores.push_back(average);
              }

              //get the average of scores for an easy to compare value for the team composition
              float average_score = 0;
              for(int l = 0; l < species.size(); l++)
              {
                average_score += average_scores[l];
              }
              average_score = average_score / species.size();

              team this_team;
              this_team.score = average_score;
              this_team.species1 = species[i];
              this_team.moveset1 = moveset[i];
              this_team.species2 = species[j];
              this_team.moveset2 = moveset[j];
              this_team.species3 = species[k];
              this_team.moveset3 = moveset[k];
              this_team.species4 = species[m];
              this_team.moveset4 = moveset[m];
              this_team.species5 = species[n];
              this_team.moveset5 = moveset[n];
              this_team.species6 = species[o];
              this_team.moveset6 = moveset[o];

              //limiting the size of the team_scores vector, so it only has the top 10,000 team combinations
              //if there are 10000 or fewer entries
              if(team_scores.size() <= 10000)
              {
                //add the current team to the teams vector
                team_scores.push_back(this_team);
                //sort the team scores
                //using insertion sort method
                //sort to ensure the lowest score is at the bottom
                for(int i = 0; i < team_scores.size(); i++)
                {
                  int j = i - 1;
                  team temp_team = team_scores[i];

                  while(j >= 0 && temp_team.score < team_scores[j].score)
                  {
                    team_scores[j + 1] = team_scores[j];
                    j--;
                  }

                  team_scores[j + 1]  = temp_team;
                }
              }
              else if(this_team.score > team_scores[0].score) //compare to the lowest score (on top)
              {
                team_scores[0].score = this_team.score;
                team_scores[0].species1 = this_team.species1;
                team_scores[0].moveset1 = this_team.moveset1;
                team_scores[0].species2 = this_team.species2;
                team_scores[0].moveset2 = this_team.moveset2;
                team_scores[0].species3 = this_team.species3;
                team_scores[0].moveset3 = this_team.moveset3;
                team_scores[0].species4 = this_team.species4;
                team_scores[0].moveset4 = this_team.moveset4;
                team_scores[0].species5 = this_team.species5;
                team_scores[0].moveset5 = this_team.moveset5;
                team_scores[0].species6 = this_team.species6;
                team_scores[0].moveset6 = this_team.moveset6;

                //sort the team scores
                //using insertion sort method
                //sort to ensure the lowest score is at the bottom
                for(int i = 0; i < team_scores.size(); i++)
                {
                  int j = i - 1;
                  team temp_team = team_scores[i];

                  while(j >= 0 && temp_team.score < team_scores[j].score)
                  {
                    team_scores[j + 1] = team_scores[j];
                    j--;
                  }

                  team_scores[j + 1]  = temp_team;
                }

              }


            }
          }
        }
      }
    }
  }



  //print out the team vector in csv format
  for(int i = 0; i < team_scores.size(); i++)
  {
    cout << team_scores[i].score << "," << team_scores[i].species1 << "," << team_scores[i].moveset1 << "," << team_scores[i].species2 << "," << team_scores[i].moveset2 << "," << team_scores[i].species3 << "," << team_scores[i].moveset3;
    cout << team_scores[i].species4 << "," << team_scores[i].moveset4 << "," << team_scores[i].species5 << "," << team_scores[i].moveset5 << "," << team_scores[i].species6 << "," << team_scores[i].moveset6 << endl;
  }

}
