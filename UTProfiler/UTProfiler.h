#ifndef UT_PROFILER_h
#define UT_PROFILER_h


#include <QString>
#include <QTextStream>
using namespace std;


//Exception
class UTProfilerException : public exception{
public:
    UTProfilerException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};


enum class Note { A, B, C, D, E, F, FX, RES, ABS, /* en cours */ EC, first=A, last=EC  };
QString noteToString(const Note& n);
Note stringToNote(const QString& n);

enum class Saison { Automne, Printemps, first=Automne, last=Printemps };
inline QTextStream& operator<<(QTextStream& f, const Saison& s) { if (s==Saison::Automne) f<<"A"; else f<<"P"; return f;}

template<typename EnumType>
class EnumIterator {
    static_assert(is_enum<EnumType>::value, "EnumType have to be an enum");
    EnumType value;
    EnumIterator(EnumType val):value(val){}
public:
    static EnumIterator getFirst() { return EnumIterator(EnumType::first); }
    bool isDone() const { return value>EnumType::last; }
    EnumType operator*() const { return value; }
    void next() { value=(EnumType)(typename std::underlying_type<EnumType>::type(value)+1); }
};

typedef EnumIterator<Note> NoteIterator;
typedef EnumIterator<Saison> SaisonIterator;

#endif
