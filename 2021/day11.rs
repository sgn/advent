use std::io;

#[derive(Copy, Clone)]
struct Octopus {
    energy: u8,
    flashing: bool,
    padding: bool,
}

fn subflash(level: &mut [[Octopus; 12]; 12], r: usize, c: usize) -> () {
    let mut o = level.get_mut(r).unwrap().get_mut(c).unwrap();
    if !o.padding && !o.flashing {
        o.energy += 1;
        if o.energy > 9 {
            o.flashing = true;
            subflash(level, r - 1, c - 1);
            subflash(level, r - 1, c);
            subflash(level, r - 1, c + 1);
            subflash(level, r, c - 1);
            subflash(level, r, c + 1);
            subflash(level, r + 1, c - 1);
            subflash(level, r + 1, c);
            subflash(level, r + 1, c + 1);
        }
    }
}

fn flash(level: &mut [[Octopus; 12]; 12], r: usize, c: usize) -> () {
    if !level[r][c].flashing && level[r][c].energy > 9 {
        subflash(level, r, c);
    }
}

fn round(level: &mut [[Octopus; 12]; 12]) -> usize {
    for r in 1..11 {
        for c in 1..11 {
            level[r][c].energy += 1;
        }
    }
    for r in 1..11 {
        for c in 1..11 {
            flash(level, r, c);
        }
    }
    let mut flashed = 0;
    for r in 1..11 {
        for c in 1..11 {
            if level[r][c].flashing {
                flashed += 1;
                level[r][c].energy = 0;
                level[r][c].flashing = false;
            }
        }
    }
    flashed
}

fn main() -> io::Result<()> {
    let mut level = [[
        Octopus {
            energy: 0,
            flashing: false,
            padding: true,
        }; 12]; 12];
    for r in 1..11 {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Ok(0) | Err(_) => break,
            Ok(_) => {
                for (i, c) in line.chars().enumerate() {
                    match c {
                        '\n' => break,
                        _ => {
                            level[r][i+1].energy = c as u8 - '0' as u8;
                            level[r][i+1].padding = false;
                        },
                    }
                }
            }
        }
    }
    let mut flashed = 0;
    let mut all = 0;
    for i in 1..101 {
        let xflash = round(&mut level);
        if xflash == 100 && all == 0 {
            all = i;
        }
        flashed += xflash;
    }
    println!("{}", flashed);
    if all != 0 {
        println!("{}", all);
    } else {
        for i in 101.. {
            let xflash = round(&mut level);
            if xflash == 100 {
                println!("{}", i);
                break;
            }
        }
    }

    Ok(())
}
