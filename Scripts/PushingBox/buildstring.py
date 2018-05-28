import sys

def main(): 
	if len(sys.argv) == 2:
		print ''.join(["key"+str(i)+"=$key"+str(i)+"$&val"+str(i)+"=$val"+str(i)+"$&" for i in xrange(int(sys.argv[1]))])[:-1]
	else:
		num_fields = int(input("Enter the number of key-value pairs (columns): "))
		print ''.join(["key"+str(i)+"=$key"+str(i)+"$&val"+str(i)+"=$val"+str(i)+"$&" for i in xrange(num_fields)])[:-1]
		
if __name__ == "__main__":
	main()