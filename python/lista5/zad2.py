class VariableNotFound(Exception):
    '''A variable in Formula is not defined in zmienna'''
    pass
class WrongConstantValue(Exception):
    '''An init value in Stala is neither True or False'''
    pass

class Formula:
    def oblicz(self,zmienne):
        pass
    def uprosc(self):
        return self
    def __add__(self,f1):
        return Or(self,f1)
    def __mul__(self,f1):
        return And(self,f1)
    def findVar(self):
        pass
    def recTaut(self,idx, v, zm):
        if len(v) == len(zm):
            return self.oblicz(zm)
        zm.append([v[idx],True])
        l = self.recTaut(idx+1,v,zm)
        zm[idx][1]=False
        r = self.recTaut(idx+1,v,zm)
        return l and r
    def tautologia(self):
        var = list(filter(lambda x : x!='',self.findVar()))
        var = list(dict.fromkeys(var))#remove duplicates
        return self.recTaut(0,var,[])
    
class Stala(Formula):
    def __init__(self,v):
        try:
            if v!=True and v!=False:
                raise WrongConstantValue
            self.val = bool(v)
        except WrongConstantValue:
            print('value',v,'is neither True or False')
            self.val = None
    def __str__(self):
        return str(self.val)
    def oblicz(self,zmienne):
        return self.val
    def findVar(self):
        return ['']

class Zmienna(Formula):
    def __init__(self,n):
        self.name = str(n)
    def __str__(self):
        return self.name
    def findVar(self):
        return [self.name]
    def oblicz(self,zmienne):
        try:
            for z in zmienne:
                if self.name in z:
                    return z[1]
            raise VariableNotFound
        except VariableNotFound:
            print('nieznana zmienna', self.name)
            return None

class Not(Formula):
    def __init__(self,f):
        self.f1 = f
    def uprosc(self):
        f1u = self.f1.uprosc()
        if type(f1u) == Stala:
            if f1u.oblicz([]) == False:
                return Stala(True)
            else:
                return Stala(False)
        return Not(f1u)
    def findVar(self):
        return self.f1.findVar()
    def __str__(self):
        return '-'+str(self.f1)
    def oblicz(self,zmienne):
        l = self.f1.oblicz(zmienne)
        if l is None:
            return None
        return not l

class And(Formula):
    def __init__(self,l,r):
        self.f1 = l
        self.f2 = r
    def uprosc(self):
        f1u = self.f1.uprosc()
        f2u = self.f2.uprosc()
        if type(f1u) == Stala:
            if f1u.oblicz([]) == False:
                return Stala(False)
            else:
                return f2u
        if type(f2u) == Stala: 
            if f2u.oblicz([]) == False:
                return Stala(False)
            else:
                return f1u
        return And(f1u,f2u)
    def findVar(self):
        l = self.f1.findVar()
        r = self.f2.findVar()
        for x in r:
            l.append(x)
        return l
    def __str__(self):
        return '(' + str(self.f1) + ' & ' + str(self.f2) + ')'
    def oblicz(self,zmienne):
        l = self.f1.oblicz(zmienne)
        r = self.f2.oblicz(zmienne)
        if l is None or r is None:
            return None
        return l and r
    
class Or(Formula):
    def __init__(self,l,r):
        self.f1 = l
        self.f2 = r
    def uprosc(self):
        f1u = self.f1.uprosc()
        f2u = self.f2.uprosc()
        if type(f1u) == Stala:
            if f1u.oblicz([]) == True:
                return Stala(True)
            else:
                return f2u
        if type(f2u) == Stala: 
            if f2u.oblicz([]) == True:
                return Stala(True)
            else:
                return f1u
        return Or(f1u,f2u)
    def findVar(self):
        l = self.f1.findVar()
        r = self.f2.findVar()
        for x in r:
            l.append(x)
        return l
    def __str__(self):
        return '(' + str(self.f1) + ' | ' + str(self.f2) + ')'
    def oblicz(self,zmienne):
        l = self.f1.oblicz(zmienne)
        r = self.f2.oblicz(zmienne)
        if l is None or r is None:
            return None
        return l or r
    
    
f = Not(Stala(False))*Stala(True)+Stala(False)
z = [['x',True]]

print(f, '.oblicz =', f.oblicz(z))
print()

g = And(And(Stala(True),Stala(False)),Or(Stala(True),Zmienna('x')))
print(g, '.uprosc =' ,g.uprosc())

h = And(And(Stala(True),Stala(True)),Or(Stala(False),Zmienna('x')))
print(h, '.uprosc =' ,h.uprosc())

k = Or(And(Stala(True),Stala(True)),Or(Stala(False),Zmienna('x')))
print(k, '.uprosc =' ,k.uprosc())

print()
fail = Zmienna('Y') + Stala(False)
print(fail, '.oblicz =', fail.oblicz(z), 'dla zmiennych =',z)
fail = Stala('prawda')
print(fail, '.oblicz =', fail.oblicz(z), 'dla zmiennych =',z)

print()
print(h, '.oblicz =', h.oblicz(z))

print()
l = And(Zmienna('x'),Zmienna('y'))

z = [['y',False],['x',False]]
print(l, '.oblicz =', l.oblicz(z), 'dla zmiennych =',z)
z = [['y',False],['x',True]]
print(l, '.oblicz =', l.oblicz(z), 'dla zmiennych =',z)
z = [['y',True],['x',False]]
print(l, '.oblicz =', l.oblicz(z), 'dla zmiennych =',z)
z = [['y',True],['x',True]]
print(l, '.oblicz =', l.oblicz(z), 'dla zmiennych =',z)

print()
taut = Or(Zmienna('y'),Or(Or(Zmienna('x'),Not(Zmienna('x'))),Zmienna('z')))
print(taut, '.tautologia = ', taut.tautologia())
nottaut = And(Zmienna('y'),Or(Or(Zmienna('x'),Not(Zmienna('x'))),Zmienna('z')))
print(nottaut, '.tautologia = ', nottaut.tautologia())

print()
print(h, '.uprosc = ',h.uprosc(), '.tautologia =' ,h.uprosc().tautologia())
