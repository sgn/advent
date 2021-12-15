use std::io;
use std::io::Write;

fn process_up(map: &Vec::<Vec::<u8>>,
                pathlen: &mut Vec::<Vec::<u32>>) -> bool {
    let h = map.len();
    let w = map[0].len();
    let mut changed = false;
    for r in (0..(h - 1)).rev() {
        /* going up from below row */
        for c in 0..w {
            let xnew = pathlen[r + 1][c] + map[r][c] as u32;
            if pathlen[r][c] > xnew {
                pathlen[r][c] = xnew;
                changed = true;
            }
        }
        /* try if going right is faster */
        for c in 1..w {
            let xnew = pathlen[r][c - 1] + map[r][c] as u32;
            if pathlen[r][c] > xnew {
                pathlen[r][c] = xnew;
                changed = true;
            }
        }
        /* try if going left is faster */
        for c in (0..(w - 1)).rev() {
            let xnew = pathlen[r][c + 1] + map[r][c] as u32;
            if pathlen[r][c] > xnew {
                pathlen[r][c] = xnew;
                changed = true;
            }
        }
    }
    changed
}

fn process_down(map: &Vec::<Vec::<u8>>,
                pathlen: &mut Vec::<Vec::<u32>>) -> bool {
    let h = map.len();
    let w = map[0].len();
    let mut changed = false;
    for r in 1..h {
        /* going down from above row */
        for c in 0..w {
            let xnew = pathlen[r - 1][c] + map[r][c] as u32;
            if pathlen[r][c] > xnew {
                pathlen[r][c] = xnew;
                changed = true;
            }
        }
        /* try if going right is faster */
        for c in 1..w {
            let xnew = pathlen[r][c - 1] + map[r][c] as u32;
            if pathlen[r][c] > xnew {
                pathlen[r][c] = xnew;
                changed = true;
            }
        }
        /* try if going left is faster */
        for c in (0..(w - 1)).rev() {
            let xnew = pathlen[r][c + 1] + map[r][c] as u32;
            if pathlen[r][c] > xnew {
                pathlen[r][c] = xnew;
                changed = true;
            }
        }
    }
    changed
}

fn main() -> io::Result<()> {
    let mut map = Vec::<Vec::<u8>>::new();
    loop {
        let mut input = String::new();
        match io::stdin().read_line(&mut input) {
            Err(_) | Ok(0) | Ok(1) => break,
            Ok(_) => {
                let row: Vec::<u8> = input.chars()
                    .filter(|c| c.is_digit(10))
                    .map(|c| c as u8 - '0' as u8)
                    .collect();
                map.push(row);
            },

        }
    }
    let h = map.len();
    let w = map[0].len();
    let maxrow = std::iter::repeat(std::u32::MAX).take(w).collect();
    let mut pathlen: Vec::<Vec::<u32>> =
        std::iter::repeat(maxrow).take(h).collect();
    pathlen[0][0] = 0;
    pathlen[1][0] = map[1][0] as u32;
    for c in 1..w {
        pathlen[0][c] = pathlen[0][c - 1] + map[0][c] as u32;
        pathlen[1][c] = pathlen[0][c] + map[1][c] as u32;
    }
    while process_down(&map, &mut pathlen)
        && process_up(&map, &mut pathlen) {
    }

    println!("{}", pathlen[h - 1][w - 1]);

    /* extend toward the right */
    for row in map.iter_mut() {
        row.reserve(w * 5);
    }
    map.reserve(h * 5);
    for r in 0..h {
        for i in 1..5 {
            for c in 0..w {
                let mut value = map[r][c] + i;
                if value > 9 { value -= 9; }
                map[r].push(value);
            }
        }
    }
    for i in 1..5 {
        for r in 0..h {
            let row = map[r].iter().map(|v| {
                let mut value = v + i;
                if value > 9 { value -= 9; }
                value
            }).collect();
            map.push(row);
        }
    }
    let h = map.len();
    let w = map[0].len();
    let maxrow = std::iter::repeat(std::u32::MAX).take(w).collect();
    let mut pathlen: Vec::<Vec::<u32>> =
        std::iter::repeat(maxrow).take(h).collect();
    pathlen[0][0] = 0;
    pathlen[1][0] = map[1][0] as u32;
    for c in 1..w {
        pathlen[0][c] = pathlen[0][c - 1] + map[0][c] as u32;
        pathlen[1][c] = pathlen[0][c] + map[1][c] as u32;
    }
    while process_down(&map, &mut pathlen)
        && process_up(&map, &mut pathlen) {
    }

    println!("{}", pathlen[h - 1][w - 1]);

    Ok(())
}
