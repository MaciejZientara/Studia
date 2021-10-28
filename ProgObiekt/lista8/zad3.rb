class Jawna
	def initialize(s)
		@s = s
	end
	def zaszyfruj(klucz)
		a=""
		@s.each_char do |i|
			a+=klucz[i]
		end
		return Zaszyfrowane.new(a)
	end
	def to_s
		return @s
	end
end


class Zaszyfrowane
	def initialize(s)
		@s = s
	end
	def odszyfruj(klucz)
		a=""
		@s.each_char do |i|
			a+=klucz[i]
		end
		return Jawna.new(a)
	end
	def to_s
		return @s
	end
end


a = Jawna.new("test")
b = a.zaszyfruj({'t'=>'y','e'=>'r','s'=>'d'})
puts b.to_s
c = b.odszyfruj({'y'=>'t','r'=>'e','d'=>'s'})
puts c.to_s

