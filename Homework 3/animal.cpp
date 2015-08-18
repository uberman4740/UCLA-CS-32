//
//  animal.cpp
//  Homework 3 (q1)
//
//  Created by Jahan Cherian on 4/30/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

class Animal        //Base Class
{
public:
    Animal(std::string name);
    virtual ~Animal();
    virtual std::string name() const;   //Every Animal has a name
    virtual void speak() const = 0;     //Pure Virtual function, as an Animal can't speak
    virtual std::string moveAction() const; //Every Animal can move - i.e. Walk
private:
    std::string m_name;
};

class Cat : public Animal       //Cat class which is a Derived class of Animal
{
public:
    Cat(std::string name):Animal(name){};
    virtual	~Cat();
    virtual void speak() const;     //Override the speak function for Cat
};

class Duck : public Animal      //Duck class which is a Derived class of Animal
{
public:
    Duck(std::string name):Animal(name){};
    virtual ~Duck();
    virtual void speak() const;     //Override the speak function for Duck
    virtual std::string moveAction() const; //Override the moveAction function for Duck
};

class Pig : public Animal       //Pig class which is a Derived class of Animal
{
public:
    Pig(std::string name, int weight):Animal(name),m_weight(weight){};  //Pigs have a weight as well
    virtual ~Pig();
    virtual void speak() const;     //Override the speak function for Pig
private:
    int m_weight;
};

Animal::Animal(std::string name)    //Set the name
:m_name(name)
{}

std::string Animal::name() const    //Return the name
{
    return m_name;
}

Animal::~Animal()   //Empty destructor
{}

std::string Animal::moveAction() const  //Most animals walk
{
    return "walk";
}

void Cat::speak() const     //Cats Meow
{
    std::cout<< "Meow";
}

Cat::~Cat()     //Destructor for Cat, which says the name of the cat as well
{
    std::cout << "Destroying " << Animal::name() << " the cat" << std::endl;
}

void Duck::speak() const    //Duck Quacks
{
    std::cout << "Quack";
}

std::string Duck::moveAction() const    //Ducks don't walk, they swim
{
        return "swim";
}

Duck::~Duck()   //Destructor for Duck, which says the name of the duck as well
{
    std::cout << "Destroying " << Animal::name() << " the duck" << std::endl;
}

void Pig::speak() const     //Pigs at or above 180 Grunt, while the others Oink
{
    if(m_weight < 180)
        std::cout << "Oink";
    else
        std::cout << "Grunt";
}

Pig::~Pig()     //Destructor for Pig, which says the name of the pig as well
{
    std::cout << "Destroying " << Animal::name() << " the pig" << std::endl;
}