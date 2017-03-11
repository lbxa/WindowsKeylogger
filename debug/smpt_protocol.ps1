Param(
      [string]$att,
      [string]$subj,
      [string]$body
      )

function send-email
{
    Param(
           [Parameter(`
           Mandatory=$true)]
            [string]$to,  
           [Parameter(`
           Mandatory=$true)]
            [string]$from,
           [Parameter(`
           Mandatory=$true)]
            [string]$password,
           [Parameter(`
           Mandatory=$true)]
            [string]$subject,
           [Parameter(`
           Mandatory=$true)]
            [string]$body,
           [Parameter(`
           Mandatory=$true)]
            [string]$attatchment
        )

    try
    {
        $msg = new-Object System.Net.Mail.MailMessage($from, $to, $subject, $body)
        $server = "smtp.gmail.com"
        if($attatchment -ne $null)
        {
            try 
            {
                $attatchment = $attachment -split ("\:\:");
                # looping though the attachments
                ForEach($val in $attatchment)
                {
                    # attach is an object with every attachment(s)
                    $attach = New-Object System.Net.Mail.Attachment($val)
                    # attachments array gets added into the msg
                    $msg.Attachments.Add($attach)
                }
            } catch
            {
                # if network fails throw exit status 2
                exit 2;
            }
            
            $client = New-Object Net.Mail.SmtpClient($server, 587)
            # make sure to enable SSL
            $client.EnableSsl = $true
            # credentials need to be applied to transfer email
            $client.Credentials = New-Object System.Net.NetworkCredential($from.split("@")[0], $password)
            # send msg
            $client.Send($msg)
            Remove-Variable -Name client
            Remove-Variable -Name password
            # exit 7 status (everything went ok)
            exit 7;
        }

    } catch 
    {
        exit 3;
    }

}

try 
{
    Send-Email
        -attachment $attatchment
        -to $to
        -body $body
        -subject $subject
        -password $password
        -from $from
} catch 
{
    # exit 4 status: (there is something wrong)
    exit 4;
}

# end of SMTP protocol script