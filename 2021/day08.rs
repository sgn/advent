use std::io;

fn mkindex(c: char) -> usize {
    c as usize - 'a' as usize
}

fn onoff(maybe: &mut [usize; 7], s: &str, r: usize) -> () {
    let _ok : Vec<usize> = s.chars().map(|c| mkindex(c)).collect();
    /* turn off bits for all values */
    for i in 0..7 {
        if !_ok.contains(&i) {
            maybe[i] = maybe[i] & (!r);
        }
    }
}

fn deduce(signal: &str, output: &Vec<&str>) -> u64 {
    let xa = 1 << 0;
    let xb = 1 << 1;
    let xc = 1 << 2;
    let xd = 1 << 3;
    let xe = 1 << 4;
    let xf = 1 << 5;
    let xg = 1 << 6;
    let x0 = xa | xb | xc |      xe | xf | xg;
    let x1 =           xc |           xf     ;
    let x2 = xa |      xc | xd | xe |      xg;
    let x3 = xa |      xc | xd |      xf | xg;
    let x4 =      xb | xc | xd |      xf     ;
    let x5 = xa | xb |      xd |      xf | xg;
    let x6 = xa | xb |      xd | xe | xf | xg;
    let x7 = xa |      xc |           xf     ;
    let x8 = xa | xb | xc | xd | xe | xf | xg;
    let x9 = xa | xb | xc | xd |      xf | xg;
    let mut maybe = [ x8; 7];
    for value in signal.split_whitespace() {
        match value.len() {
            2 => onoff(&mut maybe, value, x1),
            3 => onoff(&mut maybe, value, x7),
            4 => onoff(&mut maybe, value, x4),
            5 => onoff(&mut maybe, value, x2 & x3 & x5),
            6 => onoff(&mut maybe, value, x0 & x6 & x9),
            _ => (),
        }
    }
    let mut finished = false;
    while !finished {
        finished = true;
        let mut changed = false;
        for i in 0..7 {
            let v = maybe[i];
            if (v & (v - 1)) == 0 {
                changed = true;
                for ii in 0..7 {
                    if ii != i {
                        maybe[ii] = maybe[ii] & (!v);
                    }
                }
            } else {
                finished = false;
            }
        }
        if changed {
            continue;
        } else {
            panic!("to be continued");
        }
    }
    output.iter()
        .map(|s| {
            let x = s.chars().map(|c| maybe[mkindex(c)]).fold(0, |a, v| (a | v));
            if x == x0 {
                0
            } else if x == x1 {
                1
            } else if x == x2 {
                2
            } else if x == x3 {
                3
            } else if x == x4 {
                4
            } else if x == x5 {
                5
            } else if x == x6 {
                6
            } else if x == x7 {
                7
            } else if x == x8 {
                8
            } else {
                9
            }
        })
        .fold(0, |a, v| a * 10 + v)
}

fn main() -> io::Result<()> {
    let x1478 = [
        "cf".len(),
        "bcdf".len(),
        "acf".len(),
        "abcdefg".len(),
    ];
    let mut count = 0;
    let mut total = 0;
    loop {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Err(_) | Ok(0) => break,
            Ok(_) => {
                let mut part = line.split('|');
                let signal = part.next().unwrap();
                let output : Vec<&str> = part.next().unwrap()
                    .split_whitespace().collect();
                count += output.iter()
                    .filter(|x| x1478.contains(&x.len()))
                    .count();
                total += deduce(signal, &output);
            }
        }
    }
    println!("{}", count);
    println!("{}", total);
    Ok(())
}
