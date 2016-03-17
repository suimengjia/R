#include "solution.h"
#include <utility>

Solution::Solution(const QDateTime &createdAt) :
    createdAt_(createdAt), inscriptions(0)
{
}

Solution::Solution(Solution&& other) :
    createdAt_(move(other.createdAt_)),
    rejets(move(other.rejets)),
    exigences(move(other.exigences)),
    preferences(move(other.preferences)),
    inscriptions(move(other.inscriptions)),
    formations(move(other.formations))
{
}

bool Solution::addRejet(const shared_ptr<UV> uv)
{
    return rejets.insert(uv).second;
}

bool Solution::addExigence(const shared_ptr<UV> uv)
{
    return exigences.insert(uv).second;
}

bool Solution::addInscription(const shared_ptr<Inscription> inscription)
{
    inscriptions.push_back(inscription);
    return true;
}

bool Solution::addFormation(shared_ptr<Formation> formation)
{
    return formations.insert(formation).second;
}

void Solution::removeFormation(shared_ptr<Formation> formation)
{
    formations.erase(formation);
}

bool Solution::addPreference(const shared_ptr<UV> uv)
{
    return preferences.insert(uv).second;
}
