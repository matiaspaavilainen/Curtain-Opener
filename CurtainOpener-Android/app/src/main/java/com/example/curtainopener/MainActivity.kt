package com.example.curtainopener

import android.os.Bundle
import android.os.Handler
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.rounded.Notifications
import androidx.compose.material.icons.rounded.Refresh
import androidx.compose.material3.BottomAppBar
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBarDefaults
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.curtainopener.ui.theme.CurtainOpenerTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        setTheme(R.style.Theme_CurtainOpener)
        super.onCreate(savedInstanceState)
        setContent {
            CurtainOpenerTheme {
                Surface(
                    modifier = Modifier
                        .fillMaxSize()
                        .background(MaterialTheme.colorScheme.background)
                ) {
                    HomePage()
                }
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun HomePage(viewModel: HttpClientViewModel = viewModel()) {
    val uiState by viewModel.uiState.collectAsStateWithLifecycle()
    // Sync time immediately, also calls getStatus

    Handler().postDelayed({
        viewModel.setArduinoTime()
    }, 1000)

    val basicTextBox = Modifier
        .border(
            width = 1.dp,
            color = MaterialTheme.colorScheme.primary,
            shape = RoundedCornerShape(size = 8.dp)
        )
        .clip(RoundedCornerShape(size = 8.dp))
        .background(MaterialTheme.colorScheme.primaryContainer)
        .padding(8.dp)

    Scaffold(
        topBar = {
            CenterAlignedTopAppBar(
                colors = TopAppBarDefaults.topAppBarColors(
                    containerColor = MaterialTheme.colorScheme.primaryContainer,
                    titleContentColor = MaterialTheme.colorScheme.primary,
                ), title = {
                    Text(
                        "Curtain Opener", maxLines = 1, overflow = TextOverflow.Ellipsis
                    )
                })
        },

        bottomBar = {
            BottomAppBar(
                actions = {
                    IconButton(onClick = { viewModel.getStatus() }) {
                        Icon(
                            Icons.Rounded.Refresh,
                            contentDescription = "Get status",
                        )
                    }
                    IconButton(onClick = { viewModel.toggleAlarm() }) {
                        Icon(
                            Icons.Rounded.Notifications,
                            contentDescription = "Toggle alarm",
                        )
                    }
                })
        },

        ) { innerPadding ->

        Column(
            verticalArrangement = Arrangement.spacedBy(8.dp),
            modifier = Modifier
                .padding(innerPadding)
                .padding(8.dp)
        ) {
            Weather(
                temperature = "${uiState.temperature} °C", humidity = "${uiState.humidity} %"
            )

            Row(
                horizontalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                Text(
                    text = "Alarm status: ${uiState.alarmStatus}",
                    modifier = basicTextBox.weight(1f)
                )

                Text(
                    text = "Time synced: ${uiState.timeCorrect}", modifier = basicTextBox.weight(1f)
                )
            }

            val openTimeDialog = remember { mutableStateOf(false) }
            val currentDay = remember { mutableStateOf("") }

            if (openTimeDialog.value) {
                TimeDialog(
                    onDismissRequest = { openTimeDialog.value = false },
                    onConfirmation = { timePickerState, daysToChange ->
                        val mutableOpeningTimes = uiState.openingTimes.toMutableMap()
                        daysToChange.forEach { day ->
                            mutableOpeningTimes[day] = "${
                                timePickerState.hour.toString().padStart(2, '0')
                            }:${timePickerState.minute.toString().padStart(2, '0')}"
                        }

                        viewModel.setOpenTimes(mutableOpeningTimes)
                        openTimeDialog.value = false
                    },
                    openTimeDialog = openTimeDialog,
                    currentDay = currentDay.value,
                    currentTime = uiState.time
                )
            }

            Column(verticalArrangement = Arrangement.spacedBy(8.dp)) {
                uiState.openingTimes.forEach { (day, time) ->
                    TimeRow(day, time, openTimeDialog, currentDay)
                }
            }
        }
    }
}

@Composable
fun TimeRow(
    day: String,
    time: String,
    openTimeDialog: MutableState<Boolean>,
    currentDay: MutableState<String>,

    ) {
    Row(
        modifier = Modifier
            .border(
                width = 1.dp,
                color = MaterialTheme.colorScheme.primary,
                shape = RoundedCornerShape(size = 8.dp)
            )
            .clip(RoundedCornerShape(size = 8.dp))
            .background(MaterialTheme.colorScheme.primaryContainer)
            .padding(8.dp)
            .clickable(true, onClick = {
                currentDay.value = day
                openTimeDialog.value = true
            })
            .fillMaxWidth()
    ) {
        Text(
            text = "${day}: $time"
        )
    }
}
  