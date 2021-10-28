class Fixnum
	def czynniki
		ret = []
		for i in 1..self
			if (self%i)==0
				ret << i
			end
		end
		return ret
	end
	def ack(m)
		if self==0
			return m+1
		elsif m==0
			return (self-1).ack(1)
		else
			return (self-1).ack(self.ack(m-1))
		end
	end
	def doskonala
		sum = 0
		for a in self.czynniki
			if a<self
				sum+=a
			end
		end
		#puts self.czynniki
		#puts sum
		return sum==self
	end
	def slownie
		tmp = self
		a = []
		while tmp>0
			case tmp%10
			when 0
				a << "zero"
			when 1
				a << "jeden"
			when 2
				a << "dwa"
			when 3
				a << "trzy"
			when 4
				a << "cztery"
			when 5
				a << "piec"
			when 6
				a << "szesc"
			when 7
				a << "siedem"
			when 8
				a << "osiem"
			when 9
				a << "dziewiec"
			end
			tmp/=10
		end
		s = ""
		for i in a.reverse()
			s += i
			s += " "
		end
		return s
	end
end

#puts 6.czynniki
#puts 2.ack(1)
#puts 5.doskonala
#puts 6.doskonala
puts 123.slownie
puts 456.slownie


