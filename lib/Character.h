#include <iostream>
#include <vector>
#include <typeinfo>
using namespace std;

class Character
{
public:
    int sd;
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

    string getName() const override
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
        string response = "Target was bite by you!\n";
        write(this->sd, response.c_str(), response.length());

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

    string getName() const override
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
        string predictionMessage = "Target character is " + target->getName() + "\n";
        write(this->sd, predictionMessage.c_str(), predictionMessage.length());
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

    string getName() const override
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
