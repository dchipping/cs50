
def main():
    h = get_height()
    for i in range(1, h+1):
        print(" "*(h-i) + "#"*i + "  " + "#"*i)
        
        
def get_height():
    h = 0
    while h <= 0 or h >= 9:
        try:
            h = int(input("Height: "))
        except:
            pass
            
    return h
    
        
if __name__ == "__main__":
    main()
    