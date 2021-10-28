
class Kolekcja
	def initialize(tab)
		@tab = tab
	end
	
	def get(i)
		return @tab[i]
	end

	def swap(i,j)
		tmp = @tab[i]
		@tab[i]=@tab[j]
		@tab[j]=tmp
		return @tab #albo true?
	end

	def length
		return @tab.size
	end

	def coll
		return @tab
	end

end

class Sortowanie

	def sort1(k) #przez wybor, zlozonosc n^2
		n = k.length()-1
		for i in 0..n
			tmp = i
			for j in (i+1)..n
				if k.get(tmp) > k.get(j)
					tmp = j
				end
			end
			k.swap(i,tmp)
		end
		return k.coll()
	end

	def sort2(k) #bombelkowe, zlozonosc n^2 (dla posortowanej n)
		n = k.length()-1
		for i in 0..n
			j = i
			while j > 0 do
				if k.get(j-1) > k.get(j)
					k.swap(j,j-1)
				end
				j-=1
			end
		end
		return k.coll()
	end

end

a = Kolekcja.new([5,3,4,0,9,8,1,6,7,2])
sort = Sortowanie.new()
print a.coll()
puts "\n"
print sort.sort1(a)
puts "\n"
print sort.sort2(a)
puts "\n"

#b = Kolekcja.new([3,1,2])
#puts b.length()
#puts b.get(1)
#b.swap(0,1)
#puts b.get(1)




