/**
 * Copyright 2021 Hendrik Weiler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Parser.h"

#include <ctime>
#include <sstream>

namespace HTMLDecorators {

    using std::stringstream;

    void DecoratorDef::setParameter(string key, string value) {
        this->Params[key] = value;
    }

    bool DecoratorDef::paramExist(string key) {
        return this->Params.find(key) != this->Params.end();
    }

    string Parser::GenerateId() {
        stringstream id {};
        srand (time(NULL));
        while (true) {
            id.str("");
            id << "htmldec" << std::time(0) << (rand() % 99999999 + 1);
            if(this->genIdsMap.find(id.str()) == this->genIdsMap.end()) {
                break;
            }
        }
        // set the id as index
        this->genIdsMap[id.str()] = 1;

        return id.str();
    }

    Parser::Parser(IdMap& map) : genIdsMap(map) {

    }

    void Parser::trim(string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    string Parser::Parse(string value, DataMap data) {

        this->DecoratorList = vector<DecoratorDef>{};

        int i = 0, j;
        char ch, ch2;
        int len = value.size();
        stringstream afterParseValue {};
        DecoratorDef currentDecorator;
        stringstream currentDecoratorName {};
        stringstream currentDecoratorParameterKey {};
        stringstream currentDecoratorParameterValue {};
        string currentDecoratorId {};
        bool afterCloseTag = true,
                inDectoratorbeforeName = false,
                inParameterDefinition = false,
                inParameterKey = false,
                inParameterValue = false,
                inParameterValueQuotes = false,
                afterDecoratorNodeConnection = false,
                afterDecoratorNodeOpenTag = false,
                atFollowingClosingTag = false,
                atFollowingOpeningTag = false,
                inVariableDefinition = false,
                inVariableDefinitionNextIsBracket = false;
        int inDecoratorInNameCounter = 0;
        int inParameterValueCounter = 0;
        stringstream inVariableName {};

        for(; i < len; ++i) {
            ch = value[i];
            if(inVariableDefinition) {
                if(ch == '}') {
                    if(inParameterValue) {
                        if(data.find(inVariableName.str()) == data.end()) {
                            currentDecorator.setParameter(
                                    currentDecoratorParameterKey.str(),
                                    "${" + inVariableName.str() + ":not found}"
                            );
                        } else {
                            currentDecorator.setParameter(
                                    currentDecoratorParameterKey.str(),
                                    data.at(inVariableName.str())
                            );
                        }
                    } else {
                        if(data.find(inVariableName.str()) == data.end()) {
                            afterParseValue << "${" << inVariableName.str() << ":not found}";
                        } else {
                            afterParseValue << data.at(inVariableName.str());
                        }
                    }

                    if(i+1 < value.size() && value[i+1]==')' && inParameterValue) {
                        inVariableDefinitionNextIsBracket = true;
                        i-=1;
                    }
                    inVariableDefinition = false;
                } else {
                    inVariableName << ch;
                }
            } else if(!inVariableDefinition && ch == '$' && i+1 < len && value[i+1] == '{') {
                int x = i - 1;
                char dollorCh;
                bool moreThanOneDollar = false;
                for(; x > 0; --x) {
                    dollorCh = value[x];
                    if(dollorCh != '$') {
                        if(moreThanOneDollar) {
                            break;
                        }
                        inVariableDefinition = true;
                        inVariableName.str("");
                        i+=1;
                        break;
                    } else {
                        moreThanOneDollar = true;
                    }
                }
            } else if(!afterCloseTag && !afterDecoratorNodeConnection) {
                if(ch == '@') {
                    afterCloseTag = true;
                    inVariableDefinitionNextIsBracket = false;
                    i-=1;
                    continue;
                }
                afterParseValue << ch;
                if(i+1 < len && ch == '<' && value[i+1] == '/') {
                    afterCloseTag = true;
                    inVariableDefinitionNextIsBracket = false;
                }
            } else if(afterDecoratorNodeConnection && afterDecoratorNodeOpenTag) {
                if(ch == ' ' || ch == '>') {
                    afterParseValue << " data-dec-id=\"" << currentDecoratorId << "\" ";
                    currentDecorator.Id = currentDecoratorId;
                    this->DecoratorList.push_back(currentDecorator);
                    afterDecoratorNodeConnection = false;
                    afterDecoratorNodeOpenTag = false;
                }
                afterParseValue << ch;
            } else if(afterDecoratorNodeConnection && !afterDecoratorNodeOpenTag) {
                if (ch == '@' || i == len-1) {
                    afterDecoratorNodeConnection = false;
                    inDectoratorbeforeName = true;

                    // set name, id and add it to the list
                    currentDecorator.Name = currentDecoratorName.str();
                    currentDecorator.Id = currentDecoratorId;
                    this->DecoratorList.push_back(currentDecorator);

                    // create a new decorator
                    DecoratorDef newDef {};
                    currentDecorator = newDef;
                    // reset name
                    currentDecoratorName.str("");

                    inParameterKey = false;
                    inParameterValue = false;
                    continue;
                }
                afterParseValue << ch;
                if (ch == '<') {
                    afterDecoratorNodeOpenTag = true;
                    afterCloseTag = false;
                }
            } else if(afterCloseTag && inParameterDefinition && inParameterValue) {
                if(inParameterValueQuotes && ch == '"' && value[i-1] != '\\') {
                    inParameterValueQuotes = false;
                    ch = '\n';
                }
                if(i+1 < len && value[i+1] == ')' && !inParameterValueQuotes) {
                    if(!currentDecorator.paramExist(currentDecoratorParameterKey.str())) {
                        stringstream paramValue {};
                        paramValue << currentDecoratorParameterValue.str();
                        if(ch != '\r' && ch != '\n') {
                            paramValue << ch;
                        }
                        if(inVariableDefinitionNextIsBracket) {
                            paramValue.str(currentDecoratorParameterValue.str());
                            inVariableDefinitionNextIsBracket = false;
                        }

                        currentDecorator.setParameter(
                                currentDecoratorParameterKey.str(),
                                paramValue.str()
                        );
                    }

                    inParameterValue = false;
                    inParameterKey = false;

                    currentDecoratorParameterValue.str("");
                    currentDecoratorParameterKey.str("");
                    continue;
                } else if(ch == ',' && !inParameterValueQuotes) {
                    if(!currentDecorator.paramExist(currentDecoratorParameterKey.str())) {
                        currentDecorator.setParameter(
                                currentDecoratorParameterKey.str(),
                                currentDecoratorParameterValue.str()
                        );
                    }

                    inParameterValue = false;
                    inParameterKey = true;
                    currentDecoratorParameterValue.str("");
                    currentDecoratorParameterKey.str("");
                    continue;
                } else if(inParameterValueCounter == 0 && ch == '"') {
                    inParameterValueQuotes = true;
                    continue;
                }

                if(ch != '\r' && ch != '\n') {
                    currentDecoratorParameterValue << ch;
                }
                inParameterValueCounter += 1;
            } else if(afterCloseTag && inParameterDefinition && inParameterKey) {
                if(ch == '=') {
                    inParameterValue = true;
                    inParameterKey = false;
                    inParameterValueCounter = 0;
                    continue;
                }
                if(ch != ' ' && ch != '\r' && ch != '\n') {
                    currentDecoratorParameterKey << ch;
                }
            } else if(afterCloseTag && inParameterDefinition && !inParameterKey) {
                if(ch == ')') {
                    afterDecoratorNodeConnection = true;
                    inParameterDefinition = false;
                } else {
                    inParameterKey = true;
                    currentDecoratorParameterKey.str("");
                    if(ch != ' ' && ch != '\r' && ch != '\n') {
                        currentDecoratorParameterKey << ch;
                    }
                }
            } else if(afterCloseTag && inDectoratorbeforeName) {
                // if the char after the @ decorator is empty skip
                if(inDecoratorInNameCounter == 0 && ch == ' ') {
                    inDectoratorbeforeName = false;
                    // if theres multiple @ skip this decorator
                } else if(inDecoratorInNameCounter == 0 && ch == '@') {
                    for(j=i+1; j < len; ++j) {
                        ch2 = value[j];
                        if(ch2=='@') {
                            afterParseValue << ch;
                            i+=1;
                        } else {
                            break;
                        }
                    }
                    afterParseValue << ch;
                    inDectoratorbeforeName = false;
                } else if(ch == '@' || i == len-1) {
                    // set name, id and add it to the list
                    currentDecorator.Name = currentDecoratorName.str();
                    currentDecorator.Id = currentDecoratorId;
                    this->DecoratorList.push_back(currentDecorator);

                    // create a new decorator
                    DecoratorDef newDef {};
                    currentDecorator = newDef;
                    // reset name
                    currentDecoratorName.str("");

                    inParameterKey = false;
                    inParameterValue = false;
                } else if(ch == '<') {
                    afterParseValue << ch;
                    currentDecorator.Name = currentDecoratorName.str();

                    afterDecoratorNodeConnection = true;
                    inParameterDefinition = false;
                    inDectoratorbeforeName = false;

                    afterDecoratorNodeOpenTag = true;
                    afterCloseTag = false;

                } else if(ch == '(') {
                    inParameterDefinition = true;
                    currentDecorator.Name = currentDecoratorName.str();
                    inDectoratorbeforeName = false;

                    inParameterKey = false;
                    inParameterValue = false;
                } else {
                    if(ch!='\n' && ch != '\r') {
                        currentDecoratorName << ch;
                        ++inDecoratorInNameCounter;
                    }
                }
            } else if(afterCloseTag && !inDectoratorbeforeName && ch == '@') {
                atFollowingClosingTag = false;
                atFollowingOpeningTag = false;
                for(j=i; j < value.size(); ++j) {
                    ch2 = value[j];
                    if(ch2 == '>' && atFollowingOpeningTag) {
                        break;
                    }
                    if(ch2 == '<' && !atFollowingOpeningTag) {
                        if(value[j+1] == '/') {
                            atFollowingClosingTag = true;
                        } else {
                            atFollowingOpeningTag = true;
                        }
                        break;
                    }
                }
                if(atFollowingClosingTag) {
                    afterParseValue << ch;
                    continue;
                }
                DecoratorDef newDef {};
                currentDecorator = newDef;
                currentDecoratorId = this->GenerateId();
                currentDecoratorName.str("");
                inDectoratorbeforeName = true;
                inParameterKey = false;
                inParameterValue = false;
                inDecoratorInNameCounter = 0;
            } else {
                afterParseValue << ch;
            }
        }

        // trim parameter values
        for(auto def : this->DecoratorList) {
            for(auto pair : def.Params) {
                this->trim(pair.second);
            }
        }

        return afterParseValue.str();

    }

    string Parser::Parse(string value) {
        DataMap data {};
        return this->Parse(value, data);
    }

}