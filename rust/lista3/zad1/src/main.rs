fn main() {
    println!("Hello, world!");
}

fn mult_and_sum(army: &str, worth: &str) -> i32 {
    let a = army.split(' ').map(|s| s.parse::<i32>().unwrap());
    let b = worth.split(' ').map(|s| s.parse::<i32>().unwrap());
    a.zip(b).map(|(a, b)| a * b).sum()
}

fn good_vs_evil(good: &str, evil: &str) -> String {
    let val_good = mult_and_sum(good,"1 2 3 3 4 10");
    let val_evil = mult_and_sum(evil,"1 2 2 2 3 5 10");
    if val_good > val_evil {
        return String::from("Battle Result: Good triumphs over Evil");
    }
    else if val_good < val_evil {
        return String::from("Battle Result: Evil eradicates all trace of Good");
    }
    else {
        return String::from("Battle Result: No victor on this battle field");
    }
}


#[test]
fn returns_expected1() {
    assert_eq!(good_vs_evil("0 0 0 0 0 10", "0 0 0 0 0 0 0"), "Battle Result: Good triumphs over Evil");
}
#[test]
fn returns_expected2() {
    assert_eq!(good_vs_evil("0 0 0 0 0 0", "0 0 0 0 0 0 10"), "Battle Result: Evil eradicates all trace of Good");
}
#[test]
fn returns_expected3() {
    assert_eq!(good_vs_evil("0 0 0 0 0 10", "0 0 0 0 0 0 10"), "Battle Result: No victor on this battle field");
}
#[test]
fn returns_expected4() {
    assert_eq!(good_vs_evil("0 2 2 0 0 0", "0 0 0 0 0 0 1"), "Battle Result: No victor on this battle field");
}
#[test]
fn returns_expected5() {
    assert_eq!(good_vs_evil("3 1 1 1 1 1", "1 1 1 1 1 1 1"), "Battle Result: No victor on this battle field");
}