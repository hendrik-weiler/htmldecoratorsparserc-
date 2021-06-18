/**
 * Copyright 2021 Hendrik Weiler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace HTMLDecorators {

    using std::string;
    using std::vector;
    using std::unordered_map;

    /**
     * The type for the map of generated ids
     *
     * @typedef
     */
    typedef unordered_map<string,int> IdMap;

    /**
     * The type for the parameters of a DecoratorDef instance
     *
     * @typedef
     */
    typedef unordered_map<string,string> Params;

    /**
     * The type for the data given to the parse function
     *
     * @typedef
     */
    typedef unordered_map<string,string> DataMap;

    /**
     * The definition class for a decorator
     *
     * @author Hendrik Weiler
     * @class
     */
    class DecoratorDef {
    public:
        /**
         * Returns the unique id of the decorator
         *
         * @var
         */
        string Id;
        /**
         * Returns the name of the decorator
         *
         * @var
         */
        string Name;
        /**
         * Returns a map of parameters
         *
         */
        Params Params;
        /**
         * Checks if a parameter was already set
         *
         * @param key The key of the parameter
         * @return True or false
         */
        bool paramExist(string key);
        /**
         * Sets a parameter
         *
         * @param key The key of the parameter
         * @param value The value of the parameter
         */
        void setParameter(string key, string value);
    };

    /**
     * Parses html and extract the decorators
     *
     * @author Hendrik Weiler
     * @class
     */
    class Parser {
    private:
        /**
         * Returns a reference to a map for storing all ids
         *
         * @var
         */
        IdMap& genIdsMap;
        /**
         * Generates an id
         *
         * @return The generated id
         */
        string GenerateId();
        /**
         * Trims a string
         *
         * @param s The string to trim
         */
        void trim(string& s);
    public:
        /**
         * The constructor
         *
         * @param map The id storage map
         */
        Parser(IdMap& map);
        /**
         * Returns a list of DecoratorDef instances
         *
         * @var
         */
        vector<DecoratorDef> DecoratorList {};
        /**
         * Parses html without data
         *
         * @param value The html to parse
         * @return The modified html
         */
        string Parse(string value);
        /**
         * Parses html with data
         *
         * @param value The html to parse
         * @param data The data to inject
         * @return The modified html
         */
        string Parse(string value, DataMap data);
    };

}