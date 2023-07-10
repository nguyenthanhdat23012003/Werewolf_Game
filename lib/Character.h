#include <iostream>
#include <vector>
#include <typeinfo>
using namespace std;

class Character
{
public:
    bool isAlive;
    int voteCount;
    int nightActionParametersCount;
    int dayActionParametersCount;

    Character(int _nightActionParametersCount, int _dayActionParametersCount)
        : isAlive(true), nightActionParametersCount(_nightActionParametersCount), dayActionParametersCount(_dayActionParametersCount)
    {
        voteCount = 0;
    }

    virtual string getName() const = 0;
    virtual void nightAction(){};
    virtual void nightAction(Character *target){};
    virtual void dayAction(){};
    virtual void dayAction(Character *target){};
    void vote(Character *target)
    {
        target->voteCount++;
        cout << getName() << " finished voting!\n";
    }
};

class Werewolf final : public Character
{
public:
    static vector<int> targetVote;

    Werewolf() : Character(1, 0) {}

    string getName() const
    {
        return "Werewolf";
    }

    void nightAction() override
    {
        cout << getName() << " performed day action!\n";
    }

    void nightAction(Character *target) override
    {
        cout << getName() << " performed night action!\n";
        target->isAlive = false;
        cout << target->getName() << " was bite by Werewolf!\n";
    }

    void dayAction() override
    {
        cout << getName() << " performed day action!\n";
    }

    void dayAction(Character *target) override
    {
        cout << getName() << " performed day action!\n";
    }
};

class Seer final : public Character
{
public:
    Seer() : Character(1, 0) {}

    string getName() const
    {
        return "Seer";
    }

    void nightAction() override
    {
        cout << getName() << " performed night action!\n";
    }

    void nightAction(Character *target) override
    {
        cout << getName() << " performed night action!\n";
        if (target->getName() == "Werewolf")
        {
            cout << "Target character is a Werewolf!\n";
        }
        else
        {
            cout << "Target character is not a Werewolf.\n";
        }
    }

    void dayAction() override
    {
        cout << getName() << " performed day action!\n";
    }

    void dayAction(Character *target) override
    {
        cout << getName() << " performed day action!\n";
    }
};

class Villager final : public Character
{
public:
    Villager() : Character(0, 0) {}

    string getName() const
    {
        return "Villager";
    }

    void nightAction() override
    {
        cout << getName() << " performed night action!\n";
    }

    void nightAction(Character *target) override
    {
        cout << getName() << " performed night action!\n";
    }

    void dayAction() override
    {
        cout << getName() << " performed day action!\n";
    }

    void dayAction(Character *target) override
    {
        cout << getName() << " performed day action!\n";
    }
};
