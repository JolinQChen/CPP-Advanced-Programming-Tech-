//
// Created by jolin chen on 11/22/19.
//

#ifndef HW6_MODEL_H
#define HW6_MODEL_H

#include <vector>
#include <string>

class Model
{
private:
    //Attribute
    std::vector<float> position_texture_normal;
    int sz;
    //Function
    std::vector<std::string> splitString(std::string str, char delimiter);
    float getFloat(std::string input);
    int getInt(std::string input);
public:
    Model(std::string objFilePath);
    ~Model();
    void Draw();
};


#endif //HW6_MODEL_H
