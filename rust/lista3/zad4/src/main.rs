fn main() {
    println!("Hello, world!");
}

fn last_digit(str1: &str, str2: &str) -> i32 {
    if str1 == "0" && str2 == "0"{
        return 1;
    }
    if str1 == "0"{
        return 0;
    }
    if str2 == "0"{
        return 1;
    }
    // 2.max(str2.len())-2 ponieważ dla len=1 dostawał odejmowanie z overflow, mały work-around
    let last_two = str2[2.max(str2.len())-2..].parse::<i32>().unwrap();
    let bmod4 = if last_two%4==0 {4} else {(last_two%4) as u32};
    let last_digit = str1[str1.len()-1..].parse::<i32>().unwrap();
    last_digit.pow(bmod4)%10
}


// https://www.geeksforgeeks.org/find-last-digit-of-ab-for-large-numbers/
// def last_digit(a, b):
//     a = int(a)
//     b = int(b)
     
//     # if a and b both are 0
//     if a == 0 and b == 0:
//         return 1
       
//     # if exponent is 0
//     if b == 0:
//         return 1
       
//     # if base is 0
//     if a == 0:
//         return 0
       
//     # if exponent is divisible by 4 that means last
//     # digit will be pow(a, 4) % 10.
//     # if exponent is not divisible by 4 that means last
//     # digit will be pow(a, b%4) % 10
//     if b % 4 == 0:
//         res = 4
//     else:
//         res = b % 4
         
//     # Find last digit in 'a' and compute its exponent
//     num = pow(a, res)
     
//     # Return last digit of num
//     return num % 10
 
// a = "117"
// b = "3"
// print(last_digit(a,b))
 
// # This code is contributed by Naimish14.



#[test]
fn returns_expected1() {
  assert_eq!(last_digit("4", "1"), 4);
}
#[test]
fn returns_expected2() {
  assert_eq!(last_digit("4", "2"), 6);
}
#[test]
fn returns_expected3() {
  assert_eq!(last_digit("9", "7"), 9);
}
#[test]
fn returns_expected4() {
  assert_eq!(last_digit("10","10000000000"), 0);
}
#[test]
fn returns_expected5() {
  assert_eq!(last_digit("1606938044258990275541962092341162602522202993782792835301376","2037035976334486086268445688409378161051468393665936250636140449354381299763336706183397376"), 6);
}
#[test]
fn returns_expected6() {
  assert_eq!(last_digit("3715290469715693021198967285016729344580685479654510946723", "68819615221552997273737174557165657483427362207517952651"), 7);
}