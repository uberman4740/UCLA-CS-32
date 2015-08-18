//
//  ScoreList.cpp
//  Homework 1 (q4)
//
//  Created by Jahan Cherian on 4/9/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#include "ScoreList.h"

ScoreList::ScoreList()
:  m_sequence()
{
}

bool ScoreList::add(unsigned long score)
{
    if (score <= 100)
    {
        m_sequence.insert(score);
        return true;
    }
    return false;
}

bool ScoreList::remove(unsigned long score)
{
    if (m_sequence.find(score) != -1)
    {
        m_sequence.erase(m_sequence.find(score));
        return true;
    }
    return false;
}

int ScoreList::size() const
{
    return m_sequence.size();
}

unsigned long ScoreList::minimum() const
{
    if (size() == 0)
    {
        return NO_SCORE;
    }
    unsigned long min_score = 0;
    unsigned long temp = 0;
    m_sequence.get(0, temp);
    
    for (int i = 0; i < size(); i++)
    {
        unsigned long temp2 = 0;
        m_sequence.get(i, temp2);
        if (temp <= temp2)
        {
            min_score = temp;
        }
        else
        {
            min_score = temp2;
        }
    }
    return min_score;
}

unsigned long ScoreList::maximum() const
{
    if (size() == 0)
    {
        return NO_SCORE;
    }
    
    unsigned long max_score = 0;
    unsigned long temp = 0;
    m_sequence.get(0, temp);
    
    for (int i = 0; i < size(); i++)
    {
        unsigned long temp2 = 0;
        m_sequence.get(i, temp2);
        if (temp >= temp2)
        {
            max_score = temp;
        }
        else
        {
            max_score = temp2;
        }
    }
    return max_score;
}
