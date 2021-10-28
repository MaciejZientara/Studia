class Funkcja
	def initialize(bl)
		@bl = bl
	end
	def value(x)
		return @bl.call(x)
	end
	def zerowe(a,b,e)
		s = (a+b)/2
		while(@bl.call(s)>e or @bl.call(s)<(-e))
			if(@bl.call(s)>e)
				s = (a+s)/2
			else
				s = (s+b)/2
			end
		end
		return s
	end
	def pole(a,b)	
		i = (b-a)/10000.0
		tmp = a
		suma = 0
		while(tmp<b)
			suma+=(@bl.call(tmp)*i)
			tmp+=i
		end
		return suma
	end
	def poch(x)
		return (@bl.call(x+0.00001)-@bl.call(x))/0.00001
	end
	def plot(a,b)			#tworzy plik graph do gnuplot
		tmp=a					#plik nalezy uzyc w gnuplot komenda:
		i=0.00001			#plot "graph" using 1:2
		open('graph','w'){ |f|
			while(tmp<=b)
				f << tmp
				f << " "
				f << @bl.call(tmp)
				f << "\n"
				tmp+=i
			end
		}
		true
	end
end


a = Funkcja.new(Proc.new {|x| x*x*x})
puts a.value(2)
b = Funkcja.new(Proc.new {|x| x-5})
#puts b.zerowe(3,8,0.0001)
puts b.pole(5,7)
puts a.poch(1)
puts b.poch(2)
#puts b.plot(0,10)
#puts a.plot(0,5)
c = Funkcja.new(Proc.new {|x| Math.sin(x)})
puts c.plot(0,10)








