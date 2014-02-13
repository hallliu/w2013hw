#!/usr/bin/awk -f
# report.awk

#the following two functions split along the angle brackets to extract the emails
function get_sender(line) {
    split(line,pieces,/<|>/)
    if(pieces[2] == "") return "[email hidden]"
    return pieces[2]
}

function get_receiver(line) {
    split(line,pieces,/<|>/)
    return pieces[5]
}

function get_score(line) { #this function assumes that line is a line containing a blocked due to spam message. It splits along the Hits: and , strings and extracts the spam score
    split(line,pieces,/Hits: /)
    split(pieces[2],stuff,/,/) #this uses the trailing comma to do another split
    return stuff[1]
}

function get_top_10(emails) {
    for(i=0; i<10; i++) {
        max_value=0 #initialize these two values here so that the scope is correct
        max_email=""
        for(e in emails) {
            if(max_value < emails[e]) {
                max_value = emails[e]
                max_email = e
            }
        }
        if(max_value == 0) break # in case the interesting values run out
        printf("%s: %d emails\n", max_email, max_value)
        emails[max_email] = 0
    }
    print("\n")
}

#avenue of attack: only look at the last line of each email record, since they contain all the info necessary and are formatted in a predictable/workable manner.
$7 ~ /Blocked/ { # seventh field on the important lines indicate blocked/passsed
    
    if ($8 ~ /SPAM/) { #if this email is spam
        senders[get_sender($0)]++
        receivers[get_receiver($0)]++
        spam++
        spamScore += get_score($0)
    }
    else if ($8 ~ /INFECTED/) {
        #not sure if infected senders/receivers should be included
        senders[get_sender($0)]++
        receivers[get_receiver($0)]++
        infected++
    }
}

$7 ~ /Passed/ { #ham case
    senders[get_sender($0)]++
    receivers[get_receiver($0)]++
    ham++
}

END {
    printf("10 top senders\n------------\n")
    get_top_10(senders)
    printf("10 top receivers\n------------\n")
    get_top_10(receivers)

    #straightforward stats calculations
    printf("Stats\n---------------\n")
    printf("Ham: %d\n", ham)
    printf("Spam: %d\n", spam)
    printf("Average Spam score: %.4f\n", spamScore/spam)
    printf("Spam to ham ratio: %.4f\n", spam/ham)
    printf("Infected: %d\n", infected)
}
# By the way, the sample report has something funky with it. The total number of emails as tallied by the top 10 receivers and as tallied by spam/ham/infected counts are different. My report appears to be consistent in this regard.
