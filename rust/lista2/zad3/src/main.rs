fn main() {
    let a = (1..100).filter(|i| i%5==0 && i%10!=0).count();
    println!("{:?}",a);
    println!("Hello, world!");
}

fn has_five_in(val : &isize) -> bool {
    let mut i = (*val).abs();
    while i > 0 {
        if i%10==5 {
            return true;
        }
        i = i/10 as isize;
    }
    return false;
}

fn dont_give_me_five(start: isize, end: isize) -> isize {
    let all = (start..=end).count();
    let five = (start..=end).filter(|i| has_five_in(i)).count();
    (all - five) as isize
}

#[test]
fn returns_expected1() {
    assert_eq!(dont_give_me_five(1, 9), 8);
}
#[test]
fn returns_expected2() {
    assert_eq!(dont_give_me_five(4, 17), 12);
}
#[test]
fn returns_expected3() {
    assert_eq!(dont_give_me_five(1, 1), 1);
}
#[test]
fn returns_expected4() {
    assert_eq!(dont_give_me_five(5, 5), 0);
}
#[test]
fn returns_expected5() {
    assert_eq!(dont_give_me_five(50,59), 0);
}
#[test]
fn returns_expected6() {
    assert_eq!(dont_give_me_five(500,599), 0);
}
#[test]
fn returns_expected7() {
    assert_eq!(dont_give_me_five(500,600), 1);
}
#[test]
fn returns_expected8() {
    assert_eq!(dont_give_me_five(-59,-50), 0);
}